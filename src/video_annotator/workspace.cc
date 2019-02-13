#include "workspace.h"

#include <iostream>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "playlist.h"

namespace tator
{
  Workspace::Workspace(QObject *parent, Playlist * playlist) :
    QObject(parent),
    currentIdx_(-1),
    newIdx_(-1),
    fileState_(NOT_SAVED),
    playlist_(playlist)
  {

  }
  
  void Workspace::validatePlaylist()
  {
    currentIdx_ = -1;
    const size_t count = playlist_->size();
    for (size_t idx = 0; idx < count; idx++)
    {
      Playlist::Status status =
	validateMP4JsonPair(playlist_->location(idx));
      
      playlist_->setStatus(idx, status);
    }
  }

  void Workspace::handleUserSelection(const QModelIndex &modelIdx)
  {
    int idx = modelIdx.row();
    if (currentIdx_ != -1)
    {
      //if (fileState_ == NOT_SAVED)
      //{
	//QDialog
	//return;
      //}
      
      Playlist::Status status =
	validateMP4JsonPair(playlist_->location(currentIdx_));
      playlist_->setStatus(currentIdx_, status);
    }

    newIdx_ = idx;
    QString mp4File = playlist_->location(idx);
    std::cout << "Loading " << mp4File.toStdString() << std::endl;
    emit requestLoadVideo(mp4File);

    // handle rest in mediaLoaded slot

  }

  void Workspace::mediaLoaded(QString filename, qreal rate)
  {
    QString jsonFile = getJSONPath(filename);
    QFileInfo info(jsonFile);
    if (info.exists())
    {
      emit requestLoadAnnotationFile(jsonFile);
      std::cout << "Loaded annotation file." << std::endl;
    }
    else
    {
      emit error(QString("Couldn't load %1").arg(jsonFile));
    }

    //Update current
    currentIdx_ = newIdx_;
    fileState_ = NOT_SAVED;
    playlist_->setStatus(newIdx_, Playlist::IN_PROCESS);
  }
  
  QString Workspace::getJSONPath(const QString &mp4FilePath)
  {
    const QString jsonExt = ".json";
    QFileInfo mp4Info(mp4FilePath);
    QString directory = mp4Info.absoluteDir().absolutePath();
    QString basename = mp4Info.baseName();
    return directory + "/" + basename + jsonExt;
  }

  Playlist::Status Workspace::validateMP4JsonPair(const QString &mp4FilePath)
  {
    Playlist::Status status = Playlist::ERROR;
    
    QString jsonFile = getJSONPath(mp4FilePath);
    QFileInfo jsonInfo(jsonFile);
    QFile json(jsonFile);
    if (jsonInfo.exists() == true && json.open(QIODevice::ReadOnly))
    {
      QByteArray bytes = json.readAll();
      json.close();
      
      // Parse JSON document
      QJsonParseError error;
      QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &error);
      if (jsonDoc.isNull())
      {
	std::cerr << "JSON Parse Error: "
		  << error.errorString().toStdString() << std::endl;
	
      }
      const QJsonObject rootObj=jsonDoc.object();
      QJsonValue detections=rootObj.value("detections");
      if (detections.isArray())
      {
	QJsonArray detectionsArray=detections.toArray();
	if (detectionsArray.count() > 0)
	{
	  bool anyDots = false;
	  for (auto detectionVar = detectionsArray.begin();
	       detectionVar != detectionsArray.end();
	       detectionVar++)
	  {
	    QJsonObject detection = detectionVar->toObject();
	    if (detection["type"].toString() == "dot")
	    {
	      anyDots = true;
	    }
	  }

	  if (anyDots == true)
	  {
	    status = Playlist::NOT_PROCESSED;
	  }
	  else
	  {
	    status = Playlist::PROCESSED;
	  }
	}
      }
    }
    
    return status;
  }
}

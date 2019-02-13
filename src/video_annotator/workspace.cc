#include "workspace.h"

#include <iostream>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>

#include "playlist.h"

namespace tator
{
  Workspace::Workspace(QWidget *parent, Playlist * playlist) :
    QObject(parent),
    parent_(parent),
    currentIdx_(-1),
    newIdx_(-1),
    playlist_(playlist),
    progressDialog_(nullptr),
    fileState_(NOT_SAVED)
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
    updateToNewIdx(idx);
  }

  void Workspace::mediaLoaded()
  {
    // If we didn't make the selection exit.
    if (newIdx_ == -1)
    {
      return;
    }

    QString mp4File = playlist_->location(newIdx_);
    QString jsonFile = getJSONPath(mp4File);
    QFileInfo info(jsonFile);
    progressDialog_->setValue(50);
    if (info.exists())
    {
      progressDialog_->setLabelText(QString("Loading %1").arg(info.fileName()));
      emit requestLoadAnnotationFile(jsonFile);
    }
    else
    {
      progressDialog_.reset();
      emit error(QString("Couldn't load %1").arg(jsonFile));
    }

    //Update current
    currentIdx_ = newIdx_;
    fileState_ = NOT_SAVED;
    playlist_->setStatus(newIdx_, Playlist::IN_PROCESS);
  }

  void Workspace::annotationFileUpdated()
  {
    progressDialog_->setValue(100);
    progressDialog_->close();
    progressDialog_.reset();
  }

  void Workspace::annotationFileSaved()
  {
    if (currentIdx_ != -1)
    {
      fileState_ = SAVED;
      QString mp4File = playlist_->location(currentIdx_);
      Playlist::Status status = validateMP4JsonPair(mp4File);
      playlist_->setStatus(currentIdx_, status);

      updateToNewIdx(currentIdx_+1);
    }
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

  void Workspace::updateToNewIdx(int idx)
  {
    if (idx >= playlist_->size())
    {
      currentIdx_=-1;
      fileState_ = NOT_SAVED;
      return;
    }

    if (currentIdx_ != -1)
    {
      if (fileState_ == NOT_SAVED)
      {
	QString jsonFile = getJSONPath(playlist_->location(currentIdx_));
	QFileInfo jsonInfo(jsonFile);
	QString msg = QString("Must save changes to '%1' before proceeding.")
	  .arg(jsonInfo.fileName());
	QMessageBox::warning(parent_, "Must save changes", msg);
	return;
      }
      
      Playlist::Status status =
	validateMP4JsonPair(playlist_->location(currentIdx_));
      playlist_->setStatus(currentIdx_, status);
    }
    
    newIdx_ = idx;
    QString mp4File = playlist_->location(idx);
    QFileInfo mp4Info(mp4File);
    if (mp4Info.exists())
    {
      std::cout << " -- Workspace: Loading " << mp4File.toStdString()
		<< std::endl;
      progressDialog_ =
	std::unique_ptr<QProgressDialog>(
	  new QProgressDialog(QString("Loading %1").arg(mp4Info.fileName()),
			      QString(), //no cancel
			      0,
			      100,
			      parent_,
			      Qt::Popup));
      
      emit requestLoadVideo(mp4File);
    }
    else
    {
      emit error(QString("Couldn't load %1").arg(mp4File));
    }
    // handle rest in mediaLoaded slot
  }
}

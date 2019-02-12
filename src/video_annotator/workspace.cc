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
  Workspace::Workspace(QObject *parent) : QObject(parent)
  {

  }
  
  void Workspace::validatePlaylist()
  {
    Playlist * playlist = qobject_cast<Playlist*>(QObject::sender());
    if (playlist)
    {
      const size_t count = playlist->size();
      for (size_t idx = 0; idx < count; idx++)
      {
	Playlist::Status status =
	  validateMP4JsonPair(playlist->location(idx));
	
        playlist->setStatus(idx, status);
      }
    }
    else
    {
      std::cerr << "ERROR: Bad sender. (" << __FILE__ << ":" << __LINE__
		<< ")" << std::endl;
    }
  }

  Playlist::Status Workspace::validateMP4JsonPair(const QString &mp4FilePath)
  {
    const QString jsonExt = ".json";
    Playlist::Status status = Playlist::ERROR;
    
    QFileInfo mp4Info(mp4FilePath);
    QString directory = mp4Info.absoluteDir().absolutePath();
    QString basename = mp4Info.baseName();
    QString jsonFile = directory + "/" + basename + jsonExt;
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

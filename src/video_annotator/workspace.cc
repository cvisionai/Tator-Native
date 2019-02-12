#include "workspace.h"

#include <iostream>

#include <QDir>
#include <QFileInfo>

#include "playlist.h"

namespace tator
{
  Workspace::Workspace(QObject *parent) : QObject(parent)
  {

  }
  
  void Workspace::validatePlaylist()
  {
    const QString jsonExt = ".json";
    Playlist * playlist = qobject_cast<Playlist*>(QObject::sender());
    if (playlist)
    {
      const size_t count = playlist->size();
      for (size_t idx = 0; idx < count; idx++)
      {
	QFileInfo mp4Info(playlist->location(idx));
	QString directory = mp4Info.absoluteDir().absolutePath();
	QString basename = mp4Info.baseName();
	QString jsonFile = directory + "/" + basename + jsonExt;
	QFileInfo jsonInfo(jsonFile);
	if (jsonInfo.exists() == true)
	{
	  
	}
	else
	{
	  playlist->setStatus(idx, Playlist::ERROR);
	}
      }
    }
    else
    {
      std::cerr << "ERROR: Bad sender. (" << __FILE__ << ":" << __LINE__
		<< ")" << std::endl;
    }
  }
}

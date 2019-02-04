#include "playlist.h"

#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

namespace tator
{
  Playlist::Playlist(QObject *parent) : QAbstractTableModel(parent)
  {
    
  }

  int Playlist::loadFromXSPF(QString filename)
  {
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);
    if (xmlFile.isOpen() == false)
    {
      error(QString("Could not open '%1'").arg(filename));
    }
    
    QXmlStreamReader xml(&xmlFile);
    while (xml.readNextStartElement())
    {
      if (xml.name() == "title")
      {
	// Do nothing with title for now
	xml.skipCurrentElement();
      }
      if (xml.name() == "trackList")
      {
	std::cout << "Processing tracklist." << std::endl;
      }
      if (xml.name() == "track")
      {
	std::cout << "Processing track." << std::endl;
	xml.skipCurrentElement();
      }
    }
    if (xml.hasError())
    {
	error(QString("Could not parse '%1'").arg(filename));
    }
    
  }

  int Playlist::rowCount(const QModelIndex &parent) const
  {
    return 1;
  }
  
  int Playlist::columnCount(const QModelIndex &parent) const
  {
    return 1;
  }
  
  QVariant Playlist::data(const QModelIndex &index,
			  int role) const
  {
    if (role == Qt::DisplayRole)
    {
      return QString("Test %1").arg(index.row());
    }

    return QVariant();
  }
  
} // end of namespace tator

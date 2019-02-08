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
      //std::cout << xml.name().toString().toStdString() << std::endl;
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
	std::cout << "Inserting track" << std::endl;;
	if (insertTrack(xml) != 0)
	{
	  error(QString("Error in '%1'. Near line=%2 col%3. ")
		.arg(filename)
		.arg(xml.lineNumber())
		.arg(xml.columnNumber())
	       );
	  
	  return -1;
	}
      }
    }
    
    if (xml.hasError())
    {
      error(QString("Could not parse '%1'").arg(filename));
      return -1;
    }

    return 0;
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

  int Playlist::insertTrack(QXmlStreamReader &xml)
  {
    auto type = xml.readNext();
    while (!(type == QXmlStreamReader::EndElement && xml.name() == "track"))
    {
      if (type == QXmlStreamReader::StartElement)
      {
	std::cout << xml.name().toString().toStdString() << std::endl;
      }
      type = xml.readNext();
    }
    std::cout << "Done Track" << std::endl;
    return 0;
  }
  
} // end of namespace tator

#include "playlist.h"

#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

namespace tator
{
  Playlist::Playlist(QObject *parent) : QAbstractTableModel(parent)
  {
    //emit headerDataChanged(
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
	// Don't need to do anything for tracklist
      }
      if (xml.name() == "track")
      {
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

    emit dataChanged(index(0,0), index(size(), 1));
    return 0;
  }

  int Playlist::rowCount(const QModelIndex &parent) const
  {
    return trackList_.size();
  }
  
  int Playlist::columnCount(const QModelIndex &parent) const
  {
    return NUM_VISIBLE_COLUMNS;
  }

  QVariant Playlist::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (role != Qt::DisplayRole)
    {
      return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
      switch (section)
      {
      case TITLE:
	return "Time";
	break;
      case ALBUM:
	return "UUID";
	break;
      }
    }

    return QVariant();
  }
  QVariant Playlist::data(const QModelIndex &index, int role) const
  { 
    if (index.isValid() == false)
    {
      return QVariant();
    }
    
    const int row = index.row();
    
    if (row > rowCount() || row < 0)
    {
      return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
      std::cout << "Processing data request @ " << index.row() << std::endl;
      switch (index.column())
      {
      case TITLE:
	return trackList_[row].title;
	break;
      case ALBUM:
	return trackList_[row].album;
	break;
      }
    }

    return QVariant();
  }

  int Playlist::insertTrack(QXmlStreamReader &xml)
  {
    trackList_.append(Track());
    struct Track &newTrack = trackList_.back();
    auto type = xml.readNext();
    while (!(type == QXmlStreamReader::EndElement && xml.name() == "track"))
    {
      if (type == QXmlStreamReader::StartElement)
      {
	if (xml.name() == "location")
	{
	  newTrack.location = xml.readElementText();
	}
	else if (xml.name() == "title")
	{
	  newTrack.title = xml.readElementText();
	}
	else if (xml.name() == "album")
	{
	  newTrack.album = xml.readElementText();
	}
	else
	{
	  // Skip other track elements.
	}
      }
      type = xml.readNext();
    }
    return 0;
  }
  
} // end of namespace tator

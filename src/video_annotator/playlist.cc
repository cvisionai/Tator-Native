#include "playlist.h"

#include <QBrush>
#include <QFile>
#include <QFont>
#include <QXmlStreamReader>

#include <iostream>

namespace tator
{
  Playlist::Playlist(QObject *parent) : QAbstractTableModel(parent)
  {
    
  }

  int Playlist::loadFromXSPF(QString filename)
  {
    trackList_.clear();
    
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
    emit playlistLoaded();
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
      if (role == Qt::SizeHintRole)
      {
	switch (section)
	{
	case TITLE:
	  return QSize(300,0);
	  break;
	case ALBUM:
	  return QSize(100,0);
	  break;
	}
      }

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

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
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

    if (role == Qt::CheckStateRole)
    {
      switch(index.column())
      {
	//Errors don't get check boxes
      case TITLE:
	switch (trackList_[row].status)
	{
	case PROCESSED:
	  return Qt::Checked;
	  break;
	case IN_PROCESS:
	  return Qt::PartiallyChecked;
	  break;
	case NOT_PROCESSED:
	  return Qt::Unchecked;
	  break;
	}

      }
    }



    // Handle formatting of text
    if (role == Qt::FontRole)
    {
      if (trackList_[row].status == ERROR)
      {	
	QFont font;
	font.setBold(true);
	return font;
      }
      if (trackList_[row].status == IN_PROCESS)
      {	
	QFont font;
	font.setItalic(true);
	return font;
      }
    }

    if (role == Qt::ForegroundRole)
    {
      if (trackList_[row].status == ERROR)
      {	
	QBrush brush(Qt::red);
	return brush;
      }

      if (trackList_[row].status == PROCESSED)
      {	
	QBrush brush(Qt::darkGray);
	return brush;
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

  Qt::ItemFlags Playlist::flags(const QModelIndex &index) const
  {
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
  }

  Playlist::Status Playlist::status(size_t idx) const
  {
    if (idx < size())
    {
	return trackList_[idx].status;
    }
  }

  int Playlist::setStatus(size_t idx, Playlist::Status status)
  {
    if (idx < size())
    {
	trackList_[idx].status = status;
    }
    else
    {
      return 0;
    }
  }
} // end of namespace tator

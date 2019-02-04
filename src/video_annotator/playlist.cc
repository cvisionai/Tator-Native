#include "playlist.h"

namespace tator
{
  Playlist::Playlist(QObject *parent) : QAbstractTableModel(parent)
  {
    
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

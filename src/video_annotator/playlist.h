#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QAbstractTableModel>
class QXmlStreamReader;
#include <QList>

namespace tator
{
  /// Parses QSFP files and presents the result as an QAbstractTableModel for viewing
  /// using appropriate QT Widgets (i.e. QTreeView, QTabelView)
  ///
  class Playlist : public QAbstractTableModel
  {
    Q_OBJECT
  public:
    Playlist(QObject *parent);

    /// Load a given XSPF file (clears current records)
    ///
    /// @param filename The location on disk of the playlist file.
    ///
    /// @returns 0 on success, -1 on failure
    int loadFromXSPF(QString filename);

    /// Overriden function to supply the number of rows(tracks) in the playlist
    ///
    /// @returns The number of tracks in the playlist. 
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /// Overriden function to supply the number of columns(visable attributes)
    /// in the playlist
    ///
    /// @returns Playlist::NUM_OF_VISABLE_COLUMNS
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /// Returns the value of the data at a given index 
    QVariant data(const QModelIndex &index,
		  int role = Qt::DisplayRole) const override;

    /// Return human readable definitions for colunmn descriptions
    QVariant headerData(int section, Qt::Orientation oritentation, int role) const override;

    /// @returns the number of elements in the playlist.
    const size_t size() const {return trackList_.size();}

  signals:
    void error(const QString &error);

  private:
    /// Struct representing the <track> object under <trackList>
    ///
    /// To enhance the viewing experience in COTs players like VLC;
    /// - "title" is displayed as a water mark in the video upon playing
    /// - "album" is displayed as a column in the playlist view.
    /// 
    struct Track
    {
      QString location; //< <location> File location on disk/server
      QString title; //< <title> Title to display of video file
      QString album; //< <album> Album of video recording
    };

    /// Enum to describe column positions
    enum 
    {
      TITLE = 0,
      ALBUM = 1,
      NUM_VISIBLE_COLUMNS
    };


    /// Given an XML file current at <track>, process the entry appending it to trackList_.
    /// 
    /// @param xml The active stream reader (with current node being '<track>')
    ///
    /// @returns 0 on success, -1 on failure.
    ///
    int insertTrack(QXmlStreamReader &xml);

    /// In-memory track list
    QList<Track> trackList_;
    
  };
} // end of namespace tator
#endif

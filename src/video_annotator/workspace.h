#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QObject>

#include "playlist.h"

namespace tator
{
  /// Workspace is a helper to Playlist to handle workflow related
  /// activities with respect to updating the model (and visual display)
  ///
  /// Specifically this class determines what state a video file is in
  /// [annotated, not annotated, error]
  class Workspace : public QObject
  {
    Q_OBJECT
  public:
    Workspace(QObject *parent, Playlist *playlist);
  public slots:
    void validatePlaylist();
    void handleUserSelection(const QModelIndex &idx);
  signals:
    void requestLoadVideo(QString file);
    
  private:
    QString getJSONPath(const QString &mp4FilePath);
    Playlist::Status validateMP4JsonPair(const QString &mp4FilePath);

    int currentIdx_;
    enum FileState
    {
      SAVED,
      NOT_SAVED
    } fileState_;

    Playlist *playlist_;
  };
}
#endif

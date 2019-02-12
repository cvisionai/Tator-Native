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
    Workspace(QObject *parent = nullptr);
  public slots:
    void validatePlaylist();
  private:
    Playlist::Status validateMP4JsonPair(const QString &mp4FilePath);
  };
}
#endif

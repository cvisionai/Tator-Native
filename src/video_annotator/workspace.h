#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QObject>

namespace tator
{
  /// Workspace is a helper to Playlist to handle workflow related
  /// activities with respect to updating the model (and visual display)
  ///
  /// Specifically this class determines what state a video file is in
  /// [annotated, not annotated, error]
  class Workspace : QObject
  {
    Q_OBJECT
  public:
    Workspace(QObject *parent = nullptr);
  };
}
#endif

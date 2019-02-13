#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QObject>
#include <QProgressDialog>

#include <memory>

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
    Workspace(QWidget *parent, Playlist *playlist);
  public slots:
    void validatePlaylist();
    void handleUserSelection(const QModelIndex &idx);
    void mediaLoaded(QString filename, qreal rate);
    void annotationFileUpdated();

  signals:
    void requestLoadVideo(QString file);
    void requestLoadAnnotationFile(QString file);
    void error(QString error);
    
  private:
    enum FileState
    {
      SAVED,
      NOT_SAVED
    }; 

    QString getJSONPath(const QString &mp4FilePath);
    Playlist::Status validateMP4JsonPair(const QString &mp4FilePath);
    QWidget *parent_;
    int currentIdx_;
    int newIdx_;
    Playlist *playlist_;
    std::unique_ptr<QProgressDialog> progressDialog_;
    FileState fileState_;
    
  };
}
#endif

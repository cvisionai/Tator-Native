#ifndef PLAYLIST_VIEW_H
#define PLAYLIST_VIEW_H

#include <memory>
#include "ui_playlist_view.h"

namespace tator
{

  class PlaylistView : public QDockWidget {
    Q_OBJECT
  public:
    explicit PlaylistView(QWidget *parent = nullptr);
  private:
    std::unique_ptr<Ui::PlaylistView> ui_;
  };

} //namespace tator
#endif

#include "playlist_view.h"

#include <chrono>

namespace tator {

  PlaylistView::PlaylistView(QWidget *parent) :
    QDockWidget(parent)
    , ui_(new Ui::PlaylistView)
    , playlist_(new Playlist(this))
  {
    ui_->setupUi(this);
    ui_->treeView->setModel(playlist_.get());
  }

  void PlaylistView::on_treeView_activated(const QModelIndex &idx)
  {
    emit selection(idx);
  }
  
}

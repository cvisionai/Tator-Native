#include "playlist_view.h"

namespace tator {

  PlaylistView::PlaylistView(QWidget *parent) :
    QDockWidget(parent)
    , ui_(new Ui::PlaylistView)
  {
    ui_->setupUi(this);
  }
}

#include "playlist_view.h"

namespace tator {

  PlaylistView::PlaylistView(QWidget *parent) :
    ui_(new Ui::PlaylistView)
  {
    setParent(parent);
    ui_->setupUi(this);
  }
}

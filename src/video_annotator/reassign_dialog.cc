#include "fish_annotator/video_annotator/reassign_dialog.h"
#include "ui_reassign_dialog.h"

namespace fish_annotator { namespace video_annotator {

ReassignDialog::ReassignDialog(QWidget *parent)
  : QDialog(parent) 
  , ui_(new Ui::ReassignDialog) {
  ui_->setupUi(this);
}

void ReassignDialog::on_ok_clicked() {
  accept();
}

void ReassignDialog::on_cancel_clicked() {
  reject();
}

void ReassignDialog::on_newTrack_stateChanged(int state) {
}

void ReassignDialog::on_framePresets_currentIndexChanged(const QString &text) {
}

Reassignment ReassignDialog::getReassignment() {
  Reassignment reassign;
  return reassign;
}

#include "../../include/fish_annotator/video_annotator/moc_reassign_dialog.cpp"

}} // namespace fish_annotator::video_annotator

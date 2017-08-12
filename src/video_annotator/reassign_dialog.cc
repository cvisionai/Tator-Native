#include <QMessageBox>

#include "reassign_dialog.h"
#include "ui_reassign_dialog.h"

namespace fish_annotator { namespace video_annotator {

enum Presets {
  kCurrentToLast,
  kFirstToCurrent,
  kEntireTrack,
  kCustom
};

ReassignDialog::ReassignDialog(
    qint64 current_frame,
    qint64 first_frame,
    qint64 last_frame,
    qint64 id,
    qint64 new_track_id,
    QWidget *parent)
  : QDialog(parent)
  , current_frame_(current_frame)
  , first_frame_(first_frame)
  , last_frame_(last_frame)
  , id_(id)
  , new_track_id_(new_track_id) 
  , reassign_()
  , ui_(new Ui::ReassignDialog) {
  setModal(true);
  ui_->setupUi(this);
  ui_->fromId->setText(QString::number(id_));
  ui_->toId->setText(QString::number(new_track_id_));
  ui_->frameStart->setText(QString::number(current_frame_));
  ui_->frameStop->setText(QString::number(last_frame_));
}

void ReassignDialog::on_ok_clicked() {
  bool ok;
  QMessageBox err;
  reassign_.from_id_ = ui_->fromId->text().toLongLong(&ok);
  if(ok == false) {
    err.setText("From ID field could not be converted to integer!");
    err.exec();
    return;
  }
  reassign_.to_id_ = ui_->toId->text().toLongLong(&ok);
  if(ok == false) {
    err.setText("To ID field could not be converted to integer!");
    err.exec();
    return;
  }
  reassign_.from_frame_ = ui_->frameStart->text().toLongLong(&ok);
  if(ok == false) {
    err.setText("From Frame field could not be converted to integer!");
    err.exec();
    return;
  }
  reassign_.to_frame_ = ui_->frameStop->text().toLongLong(&ok);
  if(ok == false) {
    err.setText("To Frame field could not be converted to integer!");
    err.exec();
    return;
  }
  accept();
}

void ReassignDialog::on_cancel_clicked() {
  reject();
}

void ReassignDialog::on_newTrack_stateChanged(int state) {
  if(state == Qt::Checked) {
    ui_->toId->setText(QString::number(new_track_id_));
    ui_->toId->setEnabled(false);
  }
  else if(state == Qt::Unchecked) {
    ui_->toId->setEnabled(true);
  }
}

void ReassignDialog::on_framePresets_currentIndexChanged(int index) {
  switch(index) {
    case kCurrentToLast:
      ui_->frameStart->setText(QString::number(current_frame_));
      ui_->frameStop->setText(QString::number(last_frame_));
      break;
    case kFirstToCurrent:
      ui_->frameStart->setText(QString::number(first_frame_));
      ui_->frameStop->setText(QString::number(current_frame_));
      break;
    case kEntireTrack:
      ui_->frameStart->setText(QString::number(first_frame_));
      ui_->frameStop->setText(QString::number(last_frame_));
      break;
  }
}

void ReassignDialog::on_fromFrame_textChanged() {
  ui_->framePresets->setCurrentIndex(kCustom);
}

void ReassignDialog::on_toFrame_textChanged() {
  ui_->framePresets->setCurrentIndex(kCustom);
}

Reassignment ReassignDialog::getReassignment() {
  return reassign_;
}

#include "moc_reassign_dialog.cpp"

}} // namespace fish_annotator::video_annotator

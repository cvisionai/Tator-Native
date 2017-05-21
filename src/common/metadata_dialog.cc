#include <QMessageBox>

#include "fish_annotator/common/metadata_dialog.h"
#include "ui_metadata_dialog.h"

namespace fish_annotator {

MetadataDialog::MetadataDialog(QWidget *parent)
  : QDialog(parent)
  , ui_(new Ui::MetadataDialog) {
  ui_->setupUi(this);
}

void MetadataDialog::on_ok_clicked() {
  QRegExp re("\\d*");
  auto trip_id_valid = re.exactMatch(ui_->tripId->text());
  auto tow_number_valid = re.exactMatch(ui_->towNumber->text());
  if(trip_id_valid == false) {
    QMessageBox msgBox;
    msgBox.setText("Trip ID must be an integer!");
    msgBox.exec();
  }
  if(tow_number_valid == false) {
    QMessageBox msgBox;
    msgBox.setText("Tow number must be an integer!");
    msgBox.exec();
  }
  if(tow_number_valid && trip_id_valid) {
    accept();
  }
}

void MetadataDialog::on_cancel_clicked() {
  reject();
}

Metadata MetadataDialog::getMetadata() {
  Metadata metadata;
  metadata.file_name_ = ui_->fileName->text().toStdString();
  metadata.reviewer_name_ = ui_->reviewerName->text().toStdString();
  metadata.trip_id_ = ui_->tripId->text().toInt();
  metadata.tow_number_ = ui_->towNumber->text().toInt();
  metadata.tow_status_ = ui_->towStatus->isChecked();
  return metadata;
}

void MetadataDialog::setMetadata(const Metadata &metadata) {
  ui_->fileName->setText(metadata.file_name_.c_str());
  ui_->reviewerName->setText(metadata.reviewer_name_.c_str());
  ui_->tripId->setText(std::to_string(metadata.trip_id_).c_str());
  ui_->towNumber->setText(std::to_string(metadata.tow_number_).c_str());
  ui_->towStatus->setChecked(metadata.tow_status_);
}

#include "../../include/fish_annotator/common/moc_metadata_dialog.cpp"

} // namespace fish_annotator


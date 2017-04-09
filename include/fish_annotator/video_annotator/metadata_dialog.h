/// @file
/// @brief Defines MetadataDialog class.

#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <memory>

#include <QWidget>
#include <QDialog>

#include "fish_annotator/video_annotator/metadata.h"
#include "ui_metadata_dialog.h"

namespace fish_annotator { namespace video_annotator {

class MetadataDialog : public QDialog {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MetadataDialog(QWidget *parent = 0);

  /// @brief Returns a Metadata object corresponding to the dialog values.
  ///
  /// @return Metadata object corresponding to the dialog values.
  Metadata getMetadata();

  /// @brief Sets dialog values to values corresponding to input metadata.
  ///
  /// @param metadata Metadata object.
  void setMetadata(const Metadata &metadata);

private slots:
  /// @brief Calls inherited accept function.
  void on_ok_clicked();

  /// @brief Calls inherited reject function.
  void on_cancel_clicked();

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::MetadataDialog> ui_;
};

}} // namespace fish_annotator::video_annotator

#endif // METADATA_DIALOG_H


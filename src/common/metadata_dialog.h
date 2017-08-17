/// @file
/// @brief Defines MetadataDialog class.

#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <memory>

#include <QWidget>
#include <QDialog>

#include "metadata.h"
#include "ui_metadata_dialog.h"

namespace fish_annotator {

class MetadataDialog : public QDialog {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  explicit MetadataDialog(QWidget *parent = 0);

  /// Returns a Metadata object corresponding to the dialog values.
  ///
  /// @return Metadata object corresponding to the dialog values.
  Metadata getMetadata();

  /// Sets dialog values to values corresponding to input metadata.
  ///
  /// @param metadata Metadata object.
  void setMetadata(const Metadata &metadata);

private slots:
  /// Calls inherited accept function.
  void on_ok_clicked();

  /// Calls inherited reject function.
  void on_cancel_clicked();

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::MetadataDialog> ui_;
};

} // namespace fish_annotator

#endif // METADATA_DIALOG_H


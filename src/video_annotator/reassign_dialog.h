/// @file
/// @brief Defines ReassignDialog class.

#ifndef REASSIGN_DIALOG_H
#define REASSIGN_DIALOG_H

#include <memory>

#include <QWidget>
#include <QDialog>

#include "ui_reassign_dialog.h"

namespace fish_annotator { namespace video_annotator {

/// Contains data needed to do an ID reassignment.
struct Reassignment {
  uint32_t from_id_; ///< ID to reassign.
  uint32_t to_id_; ///< ID to reassign to.
  uint64_t from_frame_; ///< Frame to start reassignment.
  uint64_t to_frame_; ///< Frame to stop reassignment.
};

/// Dialog for specifying ID reassignment parameters.
class ReassignDialog : public QDialog {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param current_frame Current frame when this dialog was opened.
  /// @param first_frame First frame where this track occurs.
  /// @param last_frame Last frame where this track occurs.
  /// @param id ID of this track.
  /// @param new_track_id First available ID of a new track.
  /// @param parent Parent widget.
  explicit ReassignDialog(
      qint64 current_frame,
      qint64 first_frame,
      qint64 last_frame,
      qint64 id,
      qint64 new_track_id,
      QWidget *parent = 0);

  /// Returns a reassignment object corresponding to the dialog values.
  ///
  /// @return Reassignment object corresponding to dialog values.
  Reassignment getReassignment();

private slots:
  /// Calls inherited accept function.
  void on_ok_clicked();

  /// Calls inherited reject function.
  void on_cancel_clicked();

  /// Updates ID for new track checked/unchecked.
  void on_newTrack_stateChanged(int state);

  /// Updates frame according to presets.
  void on_framePresets_currentIndexChanged(int index);

  /// Changes frame preset selection.
  void on_fromFrame_textChanged();

  /// Changes frame preset selection.
  void on_toFrame_textChanged();

private:
  /// Current frame.
  qint64 current_frame_;

  /// First frame.
  qint64 first_frame_;

  /// Last frame.
  qint64 last_frame_;

  /// ID of fish to be reassigned.
  qint64 id_;

  /// ID of new track.
  qint64 new_track_id_;

  /// Struct representing the reassignment.
  Reassignment reassign_;

  /// Dialog loaded from ui file.
  std::unique_ptr<Ui::ReassignDialog> ui_;
};

}} // namespace fish_annotator::video_annotator

#endif // REASSIGN_DIALOG_H

#ifndef GLOBAL_STATE_WIDGET_H
#define GLOBAL_STATE_WIDGET_H

#include <memory>

#include <QWidget>
#include <QCheckBox>

#include "global_state_annotation.h"
#include "ui_global_state_widget.h"

namespace fish_annotator {

/// Represents global states.
class GlobalStateWidget : public QWidget {
  Q_OBJECT
public:
  /// Constructor.
  explicit GlobalStateWidget(QWidget *parent = 0);

  /// Sets widget contents to match underlying data.
  ///
  /// @param states Shared pointer to global state annotation.
  void setStates(std::shared_ptr<GlobalStateAnnotation> states);

signals:
  /// Indicates that global state annotations have changed.
  void stateChanged();

public slots:
  /// Updates shared pointer to bool global states.
  ///
  /// @param checked True if checked.
  void updateBoolState(int checked);

  /// Updates shared pointer to string global states.
  ///
  /// @param checked True if checked.
  void updateStringState(const QPair<QString, QString> &name_and_value);

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::GlobalStateWidget> ui_;

  /// Shared pointer to global state.
  std::shared_ptr<GlobalStateAnnotation> states_;
};

} // namespace fish_annotator

#endif // GLOBAL_STATE_WIDGET_H

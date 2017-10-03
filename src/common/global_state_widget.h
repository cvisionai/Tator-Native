#ifndef GLOBAL_STATE_WIDGET_H
#define GLOBAL_STATE_WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QList>

#include <memory>
#include <iostream>
#include <fstream>
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

public slots:
  /// Updates shared pointer to global states.
  ///
  /// @param checked True if checked.
  void updateGlobalState(int checked);

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::GlobalStateWidget> ui_;

  /// Shared pointer to global state.
  std::shared_ptr<GlobalStateAnnotation> states_;
};

} // namespace fish_annotator

#endif // GLOBAL_STATE_WIDGET_H

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

/// Gives access to underlying state name.
class GlobalStateCheckBox : public QCheckBox {
  Q_OBJECT
public:
  /// Constructor.
  QCheckBox(const QString &text, QWidget *parent=Q_NULLPTR);
private:
  QString name_; /// Name of the global state.
};

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
private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::GlobalStateWidget> ui_;

  /// Shared pointer to global state.
  std::shared_ptr<GlobalStateAnnotation> states_;
};

} // namespace fish_annotator

#endif // GLOBAL_STATE_WIDGET_H

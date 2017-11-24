#ifndef STRING_STATE_WIDGET_H
#define STRING_STATE_WIDGET_H

#include <memory>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

#include "ui_string_state_widget.h"

namespace fish_annotator {

/// Represents a single global state with string type.
class StringStateWidget : public QWidget {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param name Name of the state.
  /// @param value Value of the state.
  /// @param parent Pointer to parent object.
  explicit StringStateWidget(
    const QString &name,
    const QString &value,
    QWidget *parent = 0);

public slots:
  /// Emits a signal including state name and value.
  void on_value_editingFinished();

signals:
  /// Emitted when return or enter key is pressed or the value loses focus.
  ///
  /// @param name_and_value Pair containing state name and value.
  void valueChanged(const QPair<QString, QString> &name_and_value);

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::StringStateWidget> ui_;
};
  
} // namespace fish_annotator

#endif // STRING_STATE_WIDGET_H

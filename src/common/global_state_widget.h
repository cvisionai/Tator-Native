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

class GlobalStateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GlobalStateWidget(QWidget *parent = 0);
    /// List of global state checkbox pointers.
    QList<QCheckBox *> global_states_;

private:
    /// Widget loaded from ui file.
    std::unique_ptr<Ui::GlobalStateWidget> ui_;
public:
    /// Adds global state checkbox to UI.
    ///
    /// @param checkbox pointer to QCheckBox instance.
    void add_checkbox(QCheckBox *checkbox);
    /// Loads global state file.
    ///
    /// @param in_file Path to input global state file.
    void loadGlobalFile(const QString &in_file);
};

} // namespace fish_annotator

#endif // GLOBAL_STATE_WIDGET_H

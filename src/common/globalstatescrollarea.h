#ifndef GLOBALSTATESCROLLAREA_H
#define GLOBALSTATESCROLLAREA_H

#include <memory>

#include <QScrollArea>
#include "ui_globalstatescrollarea.h"

namespace fish_annotator {
class GlobalStateScrollArea;
}

class GlobalStateScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit GlobalStateScrollArea(QWidget *parent = 0);
    ~GlobalStateScrollArea();

private:
    std::unique_ptr<Ui::GlobalStateScrollArea> ui_;
};

#endif // GLOBALSTATESCROLLAREA_H

#include "globalstatescrollarea.h"
#include "ui_globalstatescrollarea.h"

GlobalStateScrollArea::GlobalStateScrollArea(QWidget *parent) :
    QScrollArea(parent),
    ui_(new Ui::GlobalStateScrollArea)
{
    ui_->setupUi(this);
}

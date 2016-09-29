#include "fish_detector/gui/navigatorwidget.h"
#include "ui_navigatorwidget.h"

namespace fish_detector { namespace gui {

NavigatorWidget::NavigatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigatorWidget)
{
    ui->setupUi(this);
}

NavigatorWidget::~NavigatorWidget()
{
}

}} // namespace fish_detector::gui


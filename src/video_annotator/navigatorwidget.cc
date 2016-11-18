#include "fish_detector/video_annotator/navigatorwidget.h"
#include "ui_navigatorwidget.h"

namespace fish_detector { namespace video_annotator {

NavigatorWidget::NavigatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigatorWidget)
{
    ui->setupUi(this);
}

NavigatorWidget::~NavigatorWidget()
{
}

#include "../../include/fish_detector/video_annotator/moc_navigatorwidget.cpp"

}} // namespace fish_detector::video_annotator


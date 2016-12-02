#include "fish_annotator/video_annotator/navigatorwidget.h"
#include "ui_navigatorwidget.h"

namespace fish_annotator { namespace video_annotator {

NavigatorWidget::NavigatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigatorWidget)
{
    ui->setupUi(this);
}

NavigatorWidget::~NavigatorWidget()
{
}

#include "../../include/fish_annotator/video_annotator/moc_navigatorwidget.cpp"

}} // namespace fish_annotator::video_annotator


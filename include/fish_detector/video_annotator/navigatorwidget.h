#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include <QWidget>
#include <memory>

namespace fish_detector { namespace video_annotator {

namespace Ui {
  class NavigatorWidget;
}

class NavigatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigatorWidget(QWidget *parent);
    ~NavigatorWidget();

public:
    std::unique_ptr<Ui::NavigatorWidget> ui;
};

}} // namespace fish_detector::video_annotator

#endif // NAVIGATORWIDGET_H

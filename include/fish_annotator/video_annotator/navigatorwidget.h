#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include <memory>

#include <QWidget>

#include "ui_navigatorwidget.h"

namespace fish_annotator { namespace video_annotator {

class NavigatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigatorWidget(QWidget *parent);
    ~NavigatorWidget();

public:
    std::unique_ptr<Ui::NavigatorWidget> ui;
};

}} // namespace fish_annotator::video_annotator

#endif // NAVIGATORWIDGET_H

#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include <QWidget>

namespace Ui {
class NavigatorWidget;
}

class NavigatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigatorWidget(QWidget *parent);
    ~NavigatorWidget();

//private slots:
//    void on_add_region_button_clicked();

//    void on_next_with_copy_button_clicked();

//    void on_next_button_clicked();

//    void on_prev_button_clicked();

public:
//    Ui::NavigatorWidget *ui;
    std::unique_ptr<Ui::NavigatorWidget> ui;
};

#endif // NAVIGATORWIDGET_H

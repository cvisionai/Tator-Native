#include "navigatorwidget.h"
#include "ui_navigatorwidget.h"
#include "mainwindow.h"

NavigatorWidget::NavigatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigatorWidget)
{
    ui->setupUi(this);
}

NavigatorWidget::~NavigatorWidget()
{
//    delete ui;
}

//void NavigatorWidget::on_add_region_button_clicked()
//{

//}

//void NavigatorWidget::on_next_with_copy_button_clicked()
//{

//}

//void NavigatorWidget::on_next_button_clicked()
//{
////    MainWindow *main = (MainWindow *)parent();
////    main->updateImage(main->getPlayer()->nextFrame());

//}

//void NavigatorWidget::on_prev_button_clicked()
//{
////    MainWindow *main = (MainWindow *)parent();
////    main->updateImage(main->getPlayer()->prevFrame());
//}

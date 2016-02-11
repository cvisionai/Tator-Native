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
}


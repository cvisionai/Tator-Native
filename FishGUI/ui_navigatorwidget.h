/********************************************************************************
** Form generated from reading UI file 'navigatorwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAVIGATORWIDGET_H
#define UI_NAVIGATORWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NavigatorWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *add_region_button;
    QPushButton *next_with_copy_button;
    QPushButton *next_button;
    QPushButton *prev_button;

    void setupUi(QWidget *NavigatorWidget)
    {
        if (NavigatorWidget->objectName().isEmpty())
            NavigatorWidget->setObjectName(QStringLiteral("NavigatorWidget"));
        NavigatorWidget->resize(170, 465);
        verticalLayout_2 = new QVBoxLayout(NavigatorWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        add_region_button = new QPushButton(NavigatorWidget);
        add_region_button->setObjectName(QStringLiteral("add_region_button"));

        verticalLayout->addWidget(add_region_button);

        next_with_copy_button = new QPushButton(NavigatorWidget);
        next_with_copy_button->setObjectName(QStringLiteral("next_with_copy_button"));

        verticalLayout->addWidget(next_with_copy_button);

        next_button = new QPushButton(NavigatorWidget);
        next_button->setObjectName(QStringLiteral("next_button"));

        verticalLayout->addWidget(next_button);

        prev_button = new QPushButton(NavigatorWidget);
        prev_button->setObjectName(QStringLiteral("prev_button"));

        verticalLayout->addWidget(prev_button);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(NavigatorWidget);

        QMetaObject::connectSlotsByName(NavigatorWidget);
    } // setupUi

    void retranslateUi(QWidget *NavigatorWidget)
    {
        NavigatorWidget->setWindowTitle(QApplication::translate("NavigatorWidget", "Form", 0));
        add_region_button->setText(QApplication::translate("NavigatorWidget", "Add Region", 0));
        next_with_copy_button->setText(QApplication::translate("NavigatorWidget", "Next+Copy Regions", 0));
        next_button->setText(QApplication::translate("NavigatorWidget", "Next", 0));
        prev_button->setText(QApplication::translate("NavigatorWidget", "Prev", 0));
    } // retranslateUi

};

namespace Ui {
    class NavigatorWidget: public Ui_NavigatorWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAVIGATORWIDGET_H

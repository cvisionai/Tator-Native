/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_7;
    QPushButton *LoadVideo;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_8;
    QVBoxLayout *VideoLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *VideoWindow;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *Rewind;
    QPushButton *Play;
    QPushButton *FastForward;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer_5;
    QPushButton *pushButton_4;
    QSpacerItem *verticalSpacer_6;
    QPushButton *pushButton_5;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(867, 654);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_7);

        LoadVideo = new QPushButton(MainWidget);
        LoadVideo->setObjectName(QString::fromUtf8("LoadVideo"));

        verticalLayout_4->addWidget(LoadVideo);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_4->addItem(horizontalSpacer_5);

        verticalSpacer_8 = new QSpacerItem(28, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_8);


        horizontalLayout_6->addLayout(verticalLayout_4);

        VideoLayout = new QVBoxLayout();
        VideoLayout->setSpacing(0);
        VideoLayout->setObjectName(QString::fromUtf8("VideoLayout"));
        VideoLayout->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(MainWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        VideoLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        VideoWindow = new QLabel(MainWidget);
        VideoWindow->setObjectName(QString::fromUtf8("VideoWindow"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VideoWindow->sizePolicy().hasHeightForWidth());
        VideoWindow->setSizePolicy(sizePolicy);
        VideoWindow->setMinimumSize(QSize(420, 360));
        VideoWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(44, 29, 255);\n"
"color: rgb(255, 249, 251);"));
        VideoWindow->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(VideoWindow);


        VideoLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        Rewind = new QPushButton(MainWidget);
        Rewind->setObjectName(QString::fromUtf8("Rewind"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Rewind->sizePolicy().hasHeightForWidth());
        Rewind->setSizePolicy(sizePolicy1);
        Rewind->setMinimumSize(QSize(1, 0));

        horizontalLayout->addWidget(Rewind);

        Play = new QPushButton(MainWidget);
        Play->setObjectName(QString::fromUtf8("Play"));
        sizePolicy1.setHeightForWidth(Play->sizePolicy().hasHeightForWidth());
        Play->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(Play);

        FastForward = new QPushButton(MainWidget);
        FastForward->setObjectName(QString::fromUtf8("FastForward"));
        FastForward->setEnabled(true);
        sizePolicy1.setHeightForWidth(FastForward->sizePolicy().hasHeightForWidth());
        FastForward->setSizePolicy(sizePolicy1);
        FastForward->setMinimumSize(QSize(1, 0));
        FastForward->setSizeIncrement(QSize(1, 0));

        horizontalLayout->addWidget(FastForward);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 1);

        horizontalLayout_5->addLayout(horizontalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 2);
        horizontalLayout_5->setStretch(2, 1);

        VideoLayout->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        VideoLayout->addItem(verticalSpacer);

        VideoLayout->setStretch(0, 1);
        VideoLayout->setStretch(1, 17);

        horizontalLayout_6->addLayout(VideoLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_3->addItem(horizontalSpacer_4);

        pushButton = new QPushButton(MainWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_3->addWidget(pushButton);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_3);

        pushButton_2 = new QPushButton(MainWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout_3->addWidget(pushButton_2);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_4);

        pushButton_3 = new QPushButton(MainWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        verticalLayout_3->addWidget(pushButton_3);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_5);

        pushButton_4 = new QPushButton(MainWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        verticalLayout_3->addWidget(pushButton_4);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_6);

        pushButton_5 = new QPushButton(MainWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy2);

        verticalLayout_3->addWidget(pushButton_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_6->addLayout(verticalLayout_3);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 7);
        horizontalLayout_6->setStretch(2, 2);

        verticalLayout->addLayout(horizontalLayout_6);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QApplication::translate("MainWidget", "Form", 0, QApplication::UnicodeUTF8));
        LoadVideo->setText(QApplication::translate("MainWidget", "Load Video", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWidget", "Video Title Goes Here", 0, QApplication::UnicodeUTF8));
        VideoWindow->setText(QApplication::translate("MainWidget", "Video Will Load Here", 0, QApplication::UnicodeUTF8));
        Rewind->setText(QApplication::translate("MainWidget", "<<", 0, QApplication::UnicodeUTF8));
        Play->setText(QApplication::translate("MainWidget", "Play", 0, QApplication::UnicodeUTF8));
        FastForward->setText(QApplication::translate("MainWidget", ">>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWidget", "Add Flounder", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWidget", "Add Cod", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWidget", "Add Haddock", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MainWidget", "Add Skate", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MainWidget", "Add Unknown", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

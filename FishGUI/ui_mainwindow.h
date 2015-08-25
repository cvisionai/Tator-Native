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
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget_5;
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
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1322, 925);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayoutWidget_5 = new QWidget(centralwidget);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(350, 170, 691, 491));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_7);

        LoadVideo = new QPushButton(horizontalLayoutWidget_5);
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
        label_2 = new QLabel(horizontalLayoutWidget_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        VideoLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        VideoWindow = new QLabel(horizontalLayoutWidget_5);
        VideoWindow->setObjectName(QString::fromUtf8("VideoWindow"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
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
        Rewind = new QPushButton(horizontalLayoutWidget_5);
        Rewind->setObjectName(QString::fromUtf8("Rewind"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Rewind->sizePolicy().hasHeightForWidth());
        Rewind->setSizePolicy(sizePolicy1);
        Rewind->setMinimumSize(QSize(1, 0));

        horizontalLayout->addWidget(Rewind);

        Play = new QPushButton(horizontalLayoutWidget_5);
        Play->setObjectName(QString::fromUtf8("Play"));
        sizePolicy1.setHeightForWidth(Play->sizePolicy().hasHeightForWidth());
        Play->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(Play);

        FastForward = new QPushButton(horizontalLayoutWidget_5);
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

        pushButton = new QPushButton(horizontalLayoutWidget_5);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_3->addWidget(pushButton);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_3);

        pushButton_2 = new QPushButton(horizontalLayoutWidget_5);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout_3->addWidget(pushButton_2);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_4);

        pushButton_3 = new QPushButton(horizontalLayoutWidget_5);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        verticalLayout_3->addWidget(pushButton_3);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_5);

        pushButton_4 = new QPushButton(horizontalLayoutWidget_5);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        verticalLayout_3->addWidget(pushButton_4);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_6);

        pushButton_5 = new QPushButton(horizontalLayoutWidget_5);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        verticalLayout_3->addWidget(pushButton_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_6->addLayout(verticalLayout_3);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 7);
        horizontalLayout_6->setStretch(2, 2);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1322, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(Play, SIGNAL(clicked()), VideoWindow, SLOT(update()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        LoadVideo->setText(QApplication::translate("MainWindow", "Load Video", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Video Title Goes Here", 0, QApplication::UnicodeUTF8));
        VideoWindow->setText(QApplication::translate("MainWindow", "Video Will Load Here", 0, QApplication::UnicodeUTF8));
        Rewind->setText(QApplication::translate("MainWindow", "<<", 0, QApplication::UnicodeUTF8));
        Play->setText(QApplication::translate("MainWindow", "Play", 0, QApplication::UnicodeUTF8));
        FastForward->setText(QApplication::translate("MainWindow", ">>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Add Flounder", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "Add Cod", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Add Haddock", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MainWindow", "Add Skate", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MainWindow", "Add Unknown", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

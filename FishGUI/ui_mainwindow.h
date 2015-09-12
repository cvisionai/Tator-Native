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
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
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
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_8;
    QVBoxLayout *VideoLayout;
    QHBoxLayout *videoTitleLayout;
    QLabel *videoTitle;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *videoWindowLayout;
    QGraphicsView *videoWindow;
    QDockWidget *dockWidget;
    QWidget *videoControlsDockWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *videoControlLayout;
    QSlider *videoSlider;
    QHBoxLayout *playLayout;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *playControlsLayout;
    QPushButton *SlowDown;
    QPushButton *Play;
    QPushButton *SpeedUp;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *loadVideoLayout;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *LoadVideo;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_11;
    QVBoxLayout *countingControlsLayout;
    QSpacerItem *verticalSpacer_10;
    QPushButton *addFlounder;
    QSpacerItem *verticalSpacer_3;
    QPushButton *addCod;
    QSpacerItem *verticalSpacer_4;
    QPushButton *addHaddock;
    QSpacerItem *verticalSpacer_5;
    QPushButton *addSkate;
    QSpacerItem *verticalSpacer_6;
    QPushButton *addUnknown;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(963, 654);
        MainWidget->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_7);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_4->addItem(horizontalSpacer_5);

        verticalSpacer_8 = new QSpacerItem(28, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_8);


        horizontalLayout_6->addLayout(verticalLayout_4);

        VideoLayout = new QVBoxLayout();
        VideoLayout->setSpacing(0);
        VideoLayout->setObjectName(QString::fromUtf8("VideoLayout"));
        VideoLayout->setSizeConstraint(QLayout::SetNoConstraint);
        videoTitleLayout = new QHBoxLayout();
        videoTitleLayout->setSpacing(0);
        videoTitleLayout->setObjectName(QString::fromUtf8("videoTitleLayout"));
        videoTitle = new QLabel(MainWidget);
        videoTitle->setObjectName(QString::fromUtf8("videoTitle"));

        videoTitleLayout->addWidget(videoTitle);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        videoTitleLayout->addItem(horizontalSpacer_3);


        VideoLayout->addLayout(videoTitleLayout);

        videoWindowLayout = new QHBoxLayout();
        videoWindowLayout->setObjectName(QString::fromUtf8("videoWindowLayout"));
        videoWindow = new QGraphicsView(MainWidget);
        videoWindow->setObjectName(QString::fromUtf8("videoWindow"));

        videoWindowLayout->addWidget(videoWindow);


        VideoLayout->addLayout(videoWindowLayout);

        dockWidget = new QDockWidget(MainWidget);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setLayoutDirection(Qt::LeftToRight);
        videoControlsDockWidget = new QWidget();
        videoControlsDockWidget->setObjectName(QString::fromUtf8("videoControlsDockWidget"));
        videoControlsDockWidget->setLayoutDirection(Qt::LeftToRight);
        videoControlsDockWidget->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(videoControlsDockWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        videoControlLayout = new QVBoxLayout();
        videoControlLayout->setSpacing(0);
        videoControlLayout->setObjectName(QString::fromUtf8("videoControlLayout"));
        videoSlider = new QSlider(videoControlsDockWidget);
        videoSlider->setObjectName(QString::fromUtf8("videoSlider"));
        videoSlider->setOrientation(Qt::Horizontal);

        videoControlLayout->addWidget(videoSlider);

        playLayout = new QHBoxLayout();
        playLayout->setObjectName(QString::fromUtf8("playLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playLayout->addItem(horizontalSpacer);

        playControlsLayout = new QHBoxLayout();
        playControlsLayout->setSpacing(2);
        playControlsLayout->setObjectName(QString::fromUtf8("playControlsLayout"));
        SlowDown = new QPushButton(videoControlsDockWidget);
        SlowDown->setObjectName(QString::fromUtf8("SlowDown"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SlowDown->sizePolicy().hasHeightForWidth());
        SlowDown->setSizePolicy(sizePolicy);
        SlowDown->setMinimumSize(QSize(1, 0));

        playControlsLayout->addWidget(SlowDown);

        Play = new QPushButton(videoControlsDockWidget);
        Play->setObjectName(QString::fromUtf8("Play"));
        sizePolicy.setHeightForWidth(Play->sizePolicy().hasHeightForWidth());
        Play->setSizePolicy(sizePolicy);

        playControlsLayout->addWidget(Play);

        SpeedUp = new QPushButton(videoControlsDockWidget);
        SpeedUp->setObjectName(QString::fromUtf8("SpeedUp"));
        SpeedUp->setEnabled(true);
        sizePolicy.setHeightForWidth(SpeedUp->sizePolicy().hasHeightForWidth());
        SpeedUp->setSizePolicy(sizePolicy);
        SpeedUp->setMinimumSize(QSize(1, 0));
        SpeedUp->setSizeIncrement(QSize(1, 0));

        playControlsLayout->addWidget(SpeedUp);

        playControlsLayout->setStretch(0, 1);
        playControlsLayout->setStretch(1, 2);
        playControlsLayout->setStretch(2, 1);

        playLayout->addLayout(playControlsLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playLayout->addItem(horizontalSpacer_2);

        playLayout->setStretch(0, 2);
        playLayout->setStretch(1, 4);
        playLayout->setStretch(2, 2);

        videoControlLayout->addLayout(playLayout);

        verticalSpacer_9 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        videoControlLayout->addItem(verticalSpacer_9);

        loadVideoLayout = new QHBoxLayout();
        loadVideoLayout->setObjectName(QString::fromUtf8("loadVideoLayout"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        loadVideoLayout->addItem(horizontalSpacer_7);

        LoadVideo = new QPushButton(videoControlsDockWidget);
        LoadVideo->setObjectName(QString::fromUtf8("LoadVideo"));
        LoadVideo->setMinimumSize(QSize(80, 0));
        LoadVideo->setMaximumSize(QSize(16777215, 16777215));

        loadVideoLayout->addWidget(LoadVideo);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        loadVideoLayout->addItem(horizontalSpacer_6);


        videoControlLayout->addLayout(loadVideoLayout);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        videoControlLayout->addItem(verticalSpacer_11);


        verticalLayout_2->addLayout(videoControlLayout);

        dockWidget->setWidget(videoControlsDockWidget);

        VideoLayout->addWidget(dockWidget);

        VideoLayout->setStretch(1, 10);
        VideoLayout->setStretch(2, 3);

        horizontalLayout_6->addLayout(VideoLayout);

        countingControlsLayout = new QVBoxLayout();
        countingControlsLayout->setSpacing(3);
        countingControlsLayout->setObjectName(QString::fromUtf8("countingControlsLayout"));
        verticalSpacer_10 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        countingControlsLayout->addItem(verticalSpacer_10);

        addFlounder = new QPushButton(MainWidget);
        addFlounder->setObjectName(QString::fromUtf8("addFlounder"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(addFlounder->sizePolicy().hasHeightForWidth());
        addFlounder->setSizePolicy(sizePolicy1);
        addFlounder->setMinimumSize(QSize(0, 50));
        addFlounder->setBaseSize(QSize(0, 0));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        addFlounder->setFont(font);
        addFlounder->setAutoFillBackground(false);
        addFlounder->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);\n"
"selection-background-color: rgb(1, 207, 111);"));
        addFlounder->setIconSize(QSize(16, 16));

        countingControlsLayout->addWidget(addFlounder);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_3);

        addCod = new QPushButton(MainWidget);
        addCod->setObjectName(QString::fromUtf8("addCod"));
        sizePolicy1.setHeightForWidth(addCod->sizePolicy().hasHeightForWidth());
        addCod->setSizePolicy(sizePolicy1);
        addCod->setMinimumSize(QSize(0, 50));
        addCod->setFont(font);
        addCod->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);\n"
"selection-background-color: rgb(1, 207, 111);"));

        countingControlsLayout->addWidget(addCod);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_4);

        addHaddock = new QPushButton(MainWidget);
        addHaddock->setObjectName(QString::fromUtf8("addHaddock"));
        sizePolicy1.setHeightForWidth(addHaddock->sizePolicy().hasHeightForWidth());
        addHaddock->setSizePolicy(sizePolicy1);
        addHaddock->setMinimumSize(QSize(0, 50));
        addHaddock->setFont(font);
        addHaddock->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);\n"
"selection-background-color: rgb(1, 207, 111);"));

        countingControlsLayout->addWidget(addHaddock);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_5);

        addSkate = new QPushButton(MainWidget);
        addSkate->setObjectName(QString::fromUtf8("addSkate"));
        sizePolicy1.setHeightForWidth(addSkate->sizePolicy().hasHeightForWidth());
        addSkate->setSizePolicy(sizePolicy1);
        addSkate->setMinimumSize(QSize(0, 50));
        addSkate->setFont(font);
        addSkate->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);\n"
"selection-background-color: rgb(1, 207, 111);"));

        countingControlsLayout->addWidget(addSkate);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_6);

        addUnknown = new QPushButton(MainWidget);
        addUnknown->setObjectName(QString::fromUtf8("addUnknown"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(addUnknown->sizePolicy().hasHeightForWidth());
        addUnknown->setSizePolicy(sizePolicy2);
        addUnknown->setMinimumSize(QSize(0, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font1.setPointSize(13);
        font1.setBold(true);
        font1.setWeight(75);
        addUnknown->setFont(font1);
        addUnknown->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);\n"
"selection-background-color: rgb(1, 207, 111);"));

        countingControlsLayout->addWidget(addUnknown);

        verticalSpacer_2 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        countingControlsLayout->addItem(verticalSpacer_2);

        countingControlsLayout->setStretch(0, 1);
        countingControlsLayout->setStretch(1, 3);
        countingControlsLayout->setStretch(2, 1);
        countingControlsLayout->setStretch(3, 3);
        countingControlsLayout->setStretch(4, 1);
        countingControlsLayout->setStretch(5, 3);
        countingControlsLayout->setStretch(6, 1);
        countingControlsLayout->setStretch(7, 3);
        countingControlsLayout->setStretch(8, 1);
        countingControlsLayout->setStretch(9, 3);
        countingControlsLayout->setStretch(10, 5);

        horizontalLayout_6->addLayout(countingControlsLayout);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 10);
        horizontalLayout_6->setStretch(2, 2);

        verticalLayout->addLayout(horizontalLayout_6);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QApplication::translate("MainWidget", "Form", 0, QApplication::UnicodeUTF8));
        videoTitle->setText(QApplication::translate("MainWidget", "Video Title Goes Here", 0, QApplication::UnicodeUTF8));
        SlowDown->setText(QApplication::translate("MainWidget", "<<", 0, QApplication::UnicodeUTF8));
        Play->setText(QApplication::translate("MainWidget", "Play", 0, QApplication::UnicodeUTF8));
        SpeedUp->setText(QApplication::translate("MainWidget", ">>", 0, QApplication::UnicodeUTF8));
        LoadVideo->setText(QApplication::translate("MainWidget", "Load Video", 0, QApplication::UnicodeUTF8));
        addFlounder->setText(QApplication::translate("MainWidget", "Flounder", 0, QApplication::UnicodeUTF8));
        addCod->setText(QApplication::translate("MainWidget", "Cod", 0, QApplication::UnicodeUTF8));
        addHaddock->setText(QApplication::translate("MainWidget", "Haddock", 0, QApplication::UnicodeUTF8));
        addSkate->setText(QApplication::translate("MainWidget", "Skate", 0, QApplication::UnicodeUTF8));
        addUnknown->setText(QApplication::translate("MainWidget", "Unknown", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

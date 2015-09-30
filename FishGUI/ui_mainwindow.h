/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFormLayout>
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
    QFormLayout *formLayout;
    QComboBox *typeMenu;
    QLabel *subTypeLabel;
    QComboBox *subTypeMenu;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *typeLabel;
    QPushButton *nextFish;
    QPushButton *updateFishFrame;
    QPushButton *prevFish;
    QPushButton *goToFrame;
    QVBoxLayout *verticalLayout_3;
    QPushButton *addRound;
    QPushButton *addFlat;
    QPushButton *addSkate;
    QPushButton *addOther;
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
    QHBoxLayout *videoSliderLayout;
    QLabel *currentTime;
    QSlider *videoSlider;
    QLabel *totalTime;
    QHBoxLayout *playLayout;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *playControlsLayout;
    QPushButton *Play;
    QPushButton *SlowDown;
    QPushButton *SpeedUp;
    QPushButton *minusOneFrame;
    QPushButton *plusOneFrame;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *loadVideoLayout;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *LoadVideo;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_11;
    QVBoxLayout *countingControlsLayout;
    QSpacerItem *verticalSpacer_10;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(901, 685);
        MainWidget->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_7 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_7);

        formLayout = new QFormLayout();
        formLayout->setContentsMargins(5, 5, 5, 5);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(5);
        typeMenu = new QComboBox(MainWidget);
        typeMenu->setObjectName(QString::fromUtf8("typeMenu"));
        typeMenu->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(0, QFormLayout::FieldRole, typeMenu);

        subTypeLabel = new QLabel(MainWidget);
        subTypeLabel->setObjectName(QString::fromUtf8("subTypeLabel"));
        QFont font;
        font.setPointSize(12);
        subTypeLabel->setFont(font);

        formLayout->setWidget(1, QFormLayout::LabelRole, subTypeLabel);

        subTypeMenu = new QComboBox(MainWidget);
        subTypeMenu->setObjectName(QString::fromUtf8("subTypeMenu"));
        subTypeMenu->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(1, QFormLayout::FieldRole, subTypeMenu);

        label_2 = new QLabel(MainWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        label = new QLabel(MainWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(2, QFormLayout::FieldRole, label);

        label_3 = new QLabel(MainWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(MainWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::FieldRole, label_4);

        typeLabel = new QLabel(MainWidget);
        typeLabel->setObjectName(QString::fromUtf8("typeLabel"));
        typeLabel->setFont(font);
        typeLabel->setLayoutDirection(Qt::LeftToRight);
        typeLabel->setAutoFillBackground(false);

        formLayout->setWidget(0, QFormLayout::LabelRole, typeLabel);

        nextFish = new QPushButton(MainWidget);
        nextFish->setObjectName(QString::fromUtf8("nextFish"));
        nextFish->setMinimumSize(QSize(100, 0));
        nextFish->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(4, QFormLayout::FieldRole, nextFish);

        updateFishFrame = new QPushButton(MainWidget);
        updateFishFrame->setObjectName(QString::fromUtf8("updateFishFrame"));
        updateFishFrame->setMinimumSize(QSize(100, 0));
        QFont font1;
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        updateFishFrame->setFont(font1);
        updateFishFrame->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(5, QFormLayout::LabelRole, updateFishFrame);

        prevFish = new QPushButton(MainWidget);
        prevFish->setObjectName(QString::fromUtf8("prevFish"));
        prevFish->setMinimumSize(QSize(100, 0));
        prevFish->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(4, QFormLayout::LabelRole, prevFish);

        goToFrame = new QPushButton(MainWidget);
        goToFrame->setObjectName(QString::fromUtf8("goToFrame"));
        goToFrame->setMinimumSize(QSize(100, 0));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        goToFrame->setFont(font2);
        goToFrame->setCursor(QCursor(Qt::ArrowCursor));
        goToFrame->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(5, QFormLayout::FieldRole, goToFrame);


        verticalLayout_4->addLayout(formLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 0, 5, -1);
        addRound = new QPushButton(MainWidget);
        addRound->setObjectName(QString::fromUtf8("addRound"));
        addRound->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addRound->sizePolicy().hasHeightForWidth());
        addRound->setSizePolicy(sizePolicy);
        addRound->setMinimumSize(QSize(0, 25));
        addRound->setBaseSize(QSize(0, 0));
        QFont font3;
        font3.setPointSize(13);
        font3.setBold(true);
        font3.setWeight(75);
        addRound->setFont(font3);
        addRound->setFocusPolicy(Qt::NoFocus);
        addRound->setAutoFillBackground(false);
        addRound->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);"));
        addRound->setIconSize(QSize(16, 16));

        verticalLayout_3->addWidget(addRound);

        addFlat = new QPushButton(MainWidget);
        addFlat->setObjectName(QString::fromUtf8("addFlat"));
        sizePolicy.setHeightForWidth(addFlat->sizePolicy().hasHeightForWidth());
        addFlat->setSizePolicy(sizePolicy);
        addFlat->setMinimumSize(QSize(0, 25));
        addFlat->setFont(font3);
        addFlat->setFocusPolicy(Qt::NoFocus);
        addFlat->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);"));

        verticalLayout_3->addWidget(addFlat);

        addSkate = new QPushButton(MainWidget);
        addSkate->setObjectName(QString::fromUtf8("addSkate"));
        sizePolicy.setHeightForWidth(addSkate->sizePolicy().hasHeightForWidth());
        addSkate->setSizePolicy(sizePolicy);
        addSkate->setMinimumSize(QSize(0, 25));
        addSkate->setFont(font3);
        addSkate->setFocusPolicy(Qt::NoFocus);
        addSkate->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);"));

        verticalLayout_3->addWidget(addSkate);

        addOther = new QPushButton(MainWidget);
        addOther->setObjectName(QString::fromUtf8("addOther"));
        addOther->setMinimumSize(QSize(0, 25));
        addOther->setFont(font3);
        addOther->setStyleSheet(QString::fromUtf8("background-color: rgb(208, 255, 228);"));

        verticalLayout_3->addWidget(addOther);


        verticalLayout_4->addLayout(verticalLayout_3);

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
        videoSliderLayout = new QHBoxLayout();
        videoSliderLayout->setObjectName(QString::fromUtf8("videoSliderLayout"));
        videoSliderLayout->setContentsMargins(-1, 0, -1, -1);
        currentTime = new QLabel(videoControlsDockWidget);
        currentTime->setObjectName(QString::fromUtf8("currentTime"));

        videoSliderLayout->addWidget(currentTime);

        videoSlider = new QSlider(videoControlsDockWidget);
        videoSlider->setObjectName(QString::fromUtf8("videoSlider"));
        videoSlider->setOrientation(Qt::Horizontal);

        videoSliderLayout->addWidget(videoSlider);

        totalTime = new QLabel(videoControlsDockWidget);
        totalTime->setObjectName(QString::fromUtf8("totalTime"));

        videoSliderLayout->addWidget(totalTime);


        videoControlLayout->addLayout(videoSliderLayout);

        playLayout = new QHBoxLayout();
        playLayout->setObjectName(QString::fromUtf8("playLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playLayout->addItem(horizontalSpacer);

        playControlsLayout = new QHBoxLayout();
        playControlsLayout->setSpacing(2);
        playControlsLayout->setObjectName(QString::fromUtf8("playControlsLayout"));
        Play = new QPushButton(videoControlsDockWidget);
        Play->setObjectName(QString::fromUtf8("Play"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Play->sizePolicy().hasHeightForWidth());
        Play->setSizePolicy(sizePolicy1);

        playControlsLayout->addWidget(Play);

        SlowDown = new QPushButton(videoControlsDockWidget);
        SlowDown->setObjectName(QString::fromUtf8("SlowDown"));
        sizePolicy1.setHeightForWidth(SlowDown->sizePolicy().hasHeightForWidth());
        SlowDown->setSizePolicy(sizePolicy1);
        SlowDown->setMinimumSize(QSize(0, 0));
        SlowDown->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(SlowDown);

        SpeedUp = new QPushButton(videoControlsDockWidget);
        SpeedUp->setObjectName(QString::fromUtf8("SpeedUp"));
        SpeedUp->setEnabled(true);
        sizePolicy1.setHeightForWidth(SpeedUp->sizePolicy().hasHeightForWidth());
        SpeedUp->setSizePolicy(sizePolicy1);
        SpeedUp->setMinimumSize(QSize(0, 0));
        SpeedUp->setSizeIncrement(QSize(0, 0));
        SpeedUp->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(SpeedUp);

        minusOneFrame = new QPushButton(videoControlsDockWidget);
        minusOneFrame->setObjectName(QString::fromUtf8("minusOneFrame"));
        minusOneFrame->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(minusOneFrame);

        plusOneFrame = new QPushButton(videoControlsDockWidget);
        plusOneFrame->setObjectName(QString::fromUtf8("plusOneFrame"));
        plusOneFrame->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(plusOneFrame);

        playControlsLayout->setStretch(0, 2);
        playControlsLayout->setStretch(1, 1);
        playControlsLayout->setStretch(2, 1);
        playControlsLayout->setStretch(3, 1);
        playControlsLayout->setStretch(4, 1);

        playLayout->addLayout(playControlsLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playLayout->addItem(horizontalSpacer_2);

        playLayout->setStretch(0, 2);
        playLayout->setStretch(1, 6);
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

        pushButton_3 = new QPushButton(videoControlsDockWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        loadVideoLayout->addWidget(pushButton_3);

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

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_4);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_5);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        countingControlsLayout->addItem(verticalSpacer_6);

        verticalSpacer_2 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        countingControlsLayout->addItem(verticalSpacer_2);

        countingControlsLayout->setStretch(0, 1);
        countingControlsLayout->setStretch(1, 1);
        countingControlsLayout->setStretch(2, 1);
        countingControlsLayout->setStretch(3, 1);
        countingControlsLayout->setStretch(4, 1);
        countingControlsLayout->setStretch(5, 5);

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
        subTypeLabel->setText(QApplication::translate("MainWidget", "SubType", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWidget", "Frame     ", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWidget", "Frame", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWidget", "UID        ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWidget", "UID", 0, QApplication::UnicodeUTF8));
        typeLabel->setText(QApplication::translate("MainWidget", "Type       ", 0, QApplication::UnicodeUTF8));
        nextFish->setText(QApplication::translate("MainWidget", "Next Fish", 0, QApplication::UnicodeUTF8));
        updateFishFrame->setText(QApplication::translate("MainWidget", "Update Frame", 0, QApplication::UnicodeUTF8));
        prevFish->setText(QApplication::translate("MainWidget", "Prev Fish", 0, QApplication::UnicodeUTF8));
        goToFrame->setText(QApplication::translate("MainWidget", "Go To Frame", 0, QApplication::UnicodeUTF8));
        addRound->setText(QApplication::translate("MainWidget", "Round", 0, QApplication::UnicodeUTF8));
        addFlat->setText(QApplication::translate("MainWidget", "Flat", 0, QApplication::UnicodeUTF8));
        addSkate->setText(QApplication::translate("MainWidget", "Skate", 0, QApplication::UnicodeUTF8));
        addOther->setText(QApplication::translate("MainWidget", "Other", 0, QApplication::UnicodeUTF8));
        videoTitle->setText(QApplication::translate("MainWidget", "Video Title Goes Here", 0, QApplication::UnicodeUTF8));
        currentTime->setText(QString());
        totalTime->setText(QString());
        Play->setText(QApplication::translate("MainWidget", "Play", 0, QApplication::UnicodeUTF8));
        SlowDown->setText(QApplication::translate("MainWidget", "Slower", 0, QApplication::UnicodeUTF8));
        SpeedUp->setText(QApplication::translate("MainWidget", "Faster", 0, QApplication::UnicodeUTF8));
        minusOneFrame->setText(QApplication::translate("MainWidget", "-1 Frame", 0, QApplication::UnicodeUTF8));
        plusOneFrame->setText(QApplication::translate("MainWidget", "+1 Frame", 0, QApplication::UnicodeUTF8));
        LoadVideo->setText(QApplication::translate("MainWidget", "Load Video", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWidget", "Load Annotation File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_7;
    QVBoxLayout *verticalLayout_5;
    QLabel *fileNameLabel;
    QLineEdit *fileNameValue;
    QHBoxLayout *horizontalLayout;
    QCheckBox *towStatus;
    QSpacerItem *horizontalSpacer_4;
    QFormLayout *formLayout_3;
    QLabel *typeLabel;
    QComboBox *typeMenu;
    QLabel *subTypeLabel;
    QComboBox *subTypeMenu;
    QFormLayout *formLayout_4;
    QLabel *fishNumLabel;
    QLabel *fishNumVal;
    QLabel *totalFishLabel;
    QLabel *totalFishVal;
    QLabel *frameCountedLabel;
    QLabel *frameCountedVal;
    QFormLayout *formLayout;
    QPushButton *prevFish;
    QPushButton *nextFish;
    QPushButton *updateFishFrame;
    QPushButton *goToFrame;
    QLabel *goToFishLabel;
    QLineEdit *goToFishVal;
    QVBoxLayout *verticalLayout_3;
    QPushButton *addRound;
    QPushButton *addFlat;
    QPushButton *addSkate;
    QPushButton *addOther;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_8;
    QVBoxLayout *VideoLayout;
    QHBoxLayout *videoTitleLayout;
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
    QPushButton *loadAnnotate;
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
            MainWidget->setObjectName(QStringLiteral("MainWidget"));
        MainWidget->resize(901, 685);
        MainWidget->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer_7 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_7);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(5, 0, 5, 5);
        fileNameLabel = new QLabel(MainWidget);
        fileNameLabel->setObjectName(QStringLiteral("fileNameLabel"));

        verticalLayout_5->addWidget(fileNameLabel);

        fileNameValue = new QLineEdit(MainWidget);
        fileNameValue->setObjectName(QStringLiteral("fileNameValue"));
        fileNameValue->setFocusPolicy(Qt::ClickFocus);

        verticalLayout_5->addWidget(fileNameValue);


        verticalLayout_4->addLayout(verticalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(19);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 5, 5, -1);
        towStatus = new QCheckBox(MainWidget);
        towStatus->setObjectName(QStringLiteral("towStatus"));
        towStatus->setLayoutDirection(Qt::RightToLeft);
        towStatus->setChecked(true);

        horizontalLayout->addWidget(towStatus);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 3);

        verticalLayout_4->addLayout(horizontalLayout);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setHorizontalSpacing(35);
        formLayout_3->setContentsMargins(5, 0, 5, 0);
        typeLabel = new QLabel(MainWidget);
        typeLabel->setObjectName(QStringLiteral("typeLabel"));
        QFont font;
        font.setPointSize(12);
        typeLabel->setFont(font);
        typeLabel->setLayoutDirection(Qt::LeftToRight);
        typeLabel->setAutoFillBackground(false);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, typeLabel);

        typeMenu = new QComboBox(MainWidget);
        typeMenu->setObjectName(QStringLiteral("typeMenu"));
        typeMenu->setFocusPolicy(Qt::NoFocus);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, typeMenu);

        subTypeLabel = new QLabel(MainWidget);
        subTypeLabel->setObjectName(QStringLiteral("subTypeLabel"));
        subTypeLabel->setFont(font);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, subTypeLabel);

        subTypeMenu = new QComboBox(MainWidget);
        subTypeMenu->setObjectName(QStringLiteral("subTypeMenu"));
        subTypeMenu->setFocusPolicy(Qt::NoFocus);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, subTypeMenu);


        verticalLayout_4->addLayout(formLayout_3);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setHorizontalSpacing(50);
        formLayout_4->setContentsMargins(5, 5, 5, 5);
        fishNumLabel = new QLabel(MainWidget);
        fishNumLabel->setObjectName(QStringLiteral("fishNumLabel"));
        fishNumLabel->setFont(font);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, fishNumLabel);

        fishNumVal = new QLabel(MainWidget);
        fishNumVal->setObjectName(QStringLiteral("fishNumVal"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, fishNumVal);

        totalFishLabel = new QLabel(MainWidget);
        totalFishLabel->setObjectName(QStringLiteral("totalFishLabel"));
        totalFishLabel->setFont(font);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, totalFishLabel);

        totalFishVal = new QLabel(MainWidget);
        totalFishVal->setObjectName(QStringLiteral("totalFishVal"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, totalFishVal);

        frameCountedLabel = new QLabel(MainWidget);
        frameCountedLabel->setObjectName(QStringLiteral("frameCountedLabel"));
        frameCountedLabel->setFont(font);

        formLayout_4->setWidget(2, QFormLayout::LabelRole, frameCountedLabel);

        frameCountedVal = new QLabel(MainWidget);
        frameCountedVal->setObjectName(QStringLiteral("frameCountedVal"));

        formLayout_4->setWidget(2, QFormLayout::FieldRole, frameCountedVal);


        verticalLayout_4->addLayout(formLayout_4);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(5);
        formLayout->setContentsMargins(5, 5, 5, 5);
        prevFish = new QPushButton(MainWidget);
        prevFish->setObjectName(QStringLiteral("prevFish"));
        prevFish->setMinimumSize(QSize(110, 0));
        prevFish->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(5, QFormLayout::LabelRole, prevFish);

        nextFish = new QPushButton(MainWidget);
        nextFish->setObjectName(QStringLiteral("nextFish"));
        nextFish->setMinimumSize(QSize(110, 0));
        nextFish->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(5, QFormLayout::FieldRole, nextFish);

        updateFishFrame = new QPushButton(MainWidget);
        updateFishFrame->setObjectName(QStringLiteral("updateFishFrame"));
        updateFishFrame->setMinimumSize(QSize(110, 0));
        QFont font1;
        font1.setPointSize(13);
        font1.setBold(false);
        font1.setWeight(50);
        updateFishFrame->setFont(font1);
        updateFishFrame->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(6, QFormLayout::LabelRole, updateFishFrame);

        goToFrame = new QPushButton(MainWidget);
        goToFrame->setObjectName(QStringLiteral("goToFrame"));
        goToFrame->setMinimumSize(QSize(110, 0));
        goToFrame->setFont(font1);
        goToFrame->setCursor(QCursor(Qt::ArrowCursor));
        goToFrame->setFocusPolicy(Qt::NoFocus);

        formLayout->setWidget(6, QFormLayout::FieldRole, goToFrame);

        goToFishLabel = new QLabel(MainWidget);
        goToFishLabel->setObjectName(QStringLiteral("goToFishLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, goToFishLabel);

        goToFishVal = new QLineEdit(MainWidget);
        goToFishVal->setObjectName(QStringLiteral("goToFishVal"));
        goToFishVal->setFocusPolicy(Qt::ClickFocus);

        formLayout->setWidget(7, QFormLayout::FieldRole, goToFishVal);


        verticalLayout_4->addLayout(formLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 0, 5, -1);
        addRound = new QPushButton(MainWidget);
        addRound->setObjectName(QStringLiteral("addRound"));
        addRound->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addRound->sizePolicy().hasHeightForWidth());
        addRound->setSizePolicy(sizePolicy);
        addRound->setMinimumSize(QSize(0, 1));
        addRound->setBaseSize(QSize(0, 0));
        QFont font2;
        font2.setPointSize(13);
        font2.setBold(true);
        font2.setWeight(75);
        addRound->setFont(font2);
        addRound->setFocusPolicy(Qt::NoFocus);
        addRound->setAutoFillBackground(false);
        addRound->setStyleSheet(QStringLiteral("background-color: rgb(208, 255, 228);"));
        addRound->setIconSize(QSize(16, 16));

        verticalLayout_3->addWidget(addRound);

        addFlat = new QPushButton(MainWidget);
        addFlat->setObjectName(QStringLiteral("addFlat"));
        sizePolicy.setHeightForWidth(addFlat->sizePolicy().hasHeightForWidth());
        addFlat->setSizePolicy(sizePolicy);
        addFlat->setMinimumSize(QSize(0, 1));
        addFlat->setFont(font2);
        addFlat->setFocusPolicy(Qt::NoFocus);
        addFlat->setStyleSheet(QStringLiteral("background-color: rgb(208, 255, 228);"));

        verticalLayout_3->addWidget(addFlat);

        addSkate = new QPushButton(MainWidget);
        addSkate->setObjectName(QStringLiteral("addSkate"));
        sizePolicy.setHeightForWidth(addSkate->sizePolicy().hasHeightForWidth());
        addSkate->setSizePolicy(sizePolicy);
        addSkate->setMinimumSize(QSize(0, 1));
        addSkate->setFont(font2);
        addSkate->setFocusPolicy(Qt::NoFocus);
        addSkate->setStyleSheet(QStringLiteral("background-color: rgb(208, 255, 228);"));

        verticalLayout_3->addWidget(addSkate);

        addOther = new QPushButton(MainWidget);
        addOther->setObjectName(QStringLiteral("addOther"));
        sizePolicy.setHeightForWidth(addOther->sizePolicy().hasHeightForWidth());
        addOther->setSizePolicy(sizePolicy);
        addOther->setMinimumSize(QSize(0, 1));
        addOther->setFont(font2);
        addOther->setStyleSheet(QStringLiteral("background-color: rgb(208, 255, 228);"));

        verticalLayout_3->addWidget(addOther);


        verticalLayout_4->addLayout(verticalLayout_3);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_4->addItem(horizontalSpacer_5);

        verticalSpacer_8 = new QSpacerItem(28, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_8);


        horizontalLayout_6->addLayout(verticalLayout_4);

        VideoLayout = new QVBoxLayout();
        VideoLayout->setSpacing(0);
        VideoLayout->setObjectName(QStringLiteral("VideoLayout"));
        VideoLayout->setSizeConstraint(QLayout::SetNoConstraint);
        videoTitleLayout = new QHBoxLayout();
        videoTitleLayout->setSpacing(0);
        videoTitleLayout->setObjectName(QStringLiteral("videoTitleLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        videoTitleLayout->addItem(horizontalSpacer_3);


        VideoLayout->addLayout(videoTitleLayout);

        videoWindowLayout = new QHBoxLayout();
        videoWindowLayout->setObjectName(QStringLiteral("videoWindowLayout"));
        videoWindow = new QGraphicsView(MainWidget);
        videoWindow->setObjectName(QStringLiteral("videoWindow"));

        videoWindowLayout->addWidget(videoWindow);


        VideoLayout->addLayout(videoWindowLayout);

        dockWidget = new QDockWidget(MainWidget);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setLayoutDirection(Qt::LeftToRight);
        videoControlsDockWidget = new QWidget();
        videoControlsDockWidget->setObjectName(QStringLiteral("videoControlsDockWidget"));
        videoControlsDockWidget->setLayoutDirection(Qt::LeftToRight);
        videoControlsDockWidget->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(videoControlsDockWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        videoControlLayout = new QVBoxLayout();
        videoControlLayout->setSpacing(0);
        videoControlLayout->setObjectName(QStringLiteral("videoControlLayout"));
        videoSliderLayout = new QHBoxLayout();
        videoSliderLayout->setObjectName(QStringLiteral("videoSliderLayout"));
        videoSliderLayout->setContentsMargins(-1, 0, -1, -1);
        currentTime = new QLabel(videoControlsDockWidget);
        currentTime->setObjectName(QStringLiteral("currentTime"));

        videoSliderLayout->addWidget(currentTime);

        videoSlider = new QSlider(videoControlsDockWidget);
        videoSlider->setObjectName(QStringLiteral("videoSlider"));
        videoSlider->setOrientation(Qt::Horizontal);

        videoSliderLayout->addWidget(videoSlider);

        totalTime = new QLabel(videoControlsDockWidget);
        totalTime->setObjectName(QStringLiteral("totalTime"));

        videoSliderLayout->addWidget(totalTime);


        videoControlLayout->addLayout(videoSliderLayout);

        playLayout = new QHBoxLayout();
        playLayout->setObjectName(QStringLiteral("playLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playLayout->addItem(horizontalSpacer);

        playControlsLayout = new QHBoxLayout();
        playControlsLayout->setSpacing(2);
        playControlsLayout->setObjectName(QStringLiteral("playControlsLayout"));
        Play = new QPushButton(videoControlsDockWidget);
        Play->setObjectName(QStringLiteral("Play"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Play->sizePolicy().hasHeightForWidth());
        Play->setSizePolicy(sizePolicy1);

        playControlsLayout->addWidget(Play);

        SlowDown = new QPushButton(videoControlsDockWidget);
        SlowDown->setObjectName(QStringLiteral("SlowDown"));
        sizePolicy1.setHeightForWidth(SlowDown->sizePolicy().hasHeightForWidth());
        SlowDown->setSizePolicy(sizePolicy1);
        SlowDown->setMinimumSize(QSize(0, 0));
        SlowDown->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(SlowDown);

        SpeedUp = new QPushButton(videoControlsDockWidget);
        SpeedUp->setObjectName(QStringLiteral("SpeedUp"));
        SpeedUp->setEnabled(true);
        sizePolicy1.setHeightForWidth(SpeedUp->sizePolicy().hasHeightForWidth());
        SpeedUp->setSizePolicy(sizePolicy1);
        SpeedUp->setMinimumSize(QSize(0, 0));
        SpeedUp->setSizeIncrement(QSize(0, 0));
        SpeedUp->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(SpeedUp);

        minusOneFrame = new QPushButton(videoControlsDockWidget);
        minusOneFrame->setObjectName(QStringLiteral("minusOneFrame"));
        minusOneFrame->setFocusPolicy(Qt::NoFocus);

        playControlsLayout->addWidget(minusOneFrame);

        plusOneFrame = new QPushButton(videoControlsDockWidget);
        plusOneFrame->setObjectName(QStringLiteral("plusOneFrame"));
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
        loadVideoLayout->setObjectName(QStringLiteral("loadVideoLayout"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        loadVideoLayout->addItem(horizontalSpacer_7);

        LoadVideo = new QPushButton(videoControlsDockWidget);
        LoadVideo->setObjectName(QStringLiteral("LoadVideo"));
        LoadVideo->setMinimumSize(QSize(80, 0));
        LoadVideo->setMaximumSize(QSize(16777215, 16777215));

        loadVideoLayout->addWidget(LoadVideo);

        loadAnnotate = new QPushButton(videoControlsDockWidget);
        loadAnnotate->setObjectName(QStringLiteral("loadAnnotate"));

        loadVideoLayout->addWidget(loadAnnotate);

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
        countingControlsLayout->setObjectName(QStringLiteral("countingControlsLayout"));
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
        MainWidget->setWindowTitle(QApplication::translate("MainWidget", "Form", 0));
        fileNameLabel->setText(QApplication::translate("MainWidget", "File Name:", 0));
        towStatus->setText(QApplication::translate("MainWidget", "Tow Open          ", 0));
        typeLabel->setText(QApplication::translate("MainWidget", "Type       ", 0));
        subTypeLabel->setText(QApplication::translate("MainWidget", "SubType", 0));
        fishNumLabel->setText(QApplication::translate("MainWidget", "Fish #", 0));
        fishNumVal->setText(QApplication::translate("MainWidget", "-", 0));
        totalFishLabel->setText(QApplication::translate("MainWidget", "Total Fish", 0));
        totalFishVal->setText(QApplication::translate("MainWidget", "-", 0));
        frameCountedLabel->setText(QApplication::translate("MainWidget", "Frame Counted", 0));
        frameCountedVal->setText(QApplication::translate("MainWidget", "-", 0));
        prevFish->setText(QApplication::translate("MainWidget", "  Prev Fish ", 0));
        nextFish->setText(QApplication::translate("MainWidget", "Next Fish", 0));
        updateFishFrame->setText(QApplication::translate("MainWidget", "Set Frame", 0));
        goToFrame->setText(QApplication::translate("MainWidget", "Go To Frame", 0));
        goToFishLabel->setText(QApplication::translate("MainWidget", "     Go To Fish   ", 0));
        addRound->setText(QApplication::translate("MainWidget", "Round", 0));
        addFlat->setText(QApplication::translate("MainWidget", "Flat", 0));
        addSkate->setText(QApplication::translate("MainWidget", "Skate", 0));
        addOther->setText(QApplication::translate("MainWidget", "Other", 0));
        currentTime->setText(QString());
        totalTime->setText(QString());
        Play->setText(QApplication::translate("MainWidget", "Play", 0));
        SlowDown->setText(QApplication::translate("MainWidget", "Slower", 0));
        SpeedUp->setText(QApplication::translate("MainWidget", "Faster", 0));
        minusOneFrame->setText(QApplication::translate("MainWidget", "-1 Frame", 0));
        plusOneFrame->setText(QApplication::translate("MainWidget", "+1 Frame", 0));
        LoadVideo->setText(QApplication::translate("MainWidget", "Load Video", 0));
        loadAnnotate->setText(QApplication::translate("MainWidget", "Load Annotation File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

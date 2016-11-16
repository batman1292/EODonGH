/********************************************************************************
** Form generated from reading UI file 'SimplePlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLEPLAYER_H
#define UI_SIMPLEPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <VLCQtWidgets/WidgetSeek.h>
#include <VLCQtWidgets/WidgetVideo.h>
#include <VLCQtWidgets/WidgetVolumeSlider.h>

QT_BEGIN_NAMESPACE

class Ui_SimplePlayer
{
public:
    QAction *actionQuit;
    QAction *actionPause;
    QAction *actionStop;
    QAction *actionOpenLocal;
    QAction *actionOpenUrl;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QPushButton *openLocal;
    QPushButton *openUrl;
    VlcWidgetVideo *video;
    QPushButton *stop;
    QPushButton *pause;
    VlcWidgetVolumeSlider *volume;
    VlcWidgetSeek *seek;
    QPushButton *equalizer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuMedia;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SimplePlayer)
    {
        if (SimplePlayer->objectName().isEmpty())
            SimplePlayer->setObjectName(QStringLiteral("SimplePlayer"));
        SimplePlayer->resize(640, 480);
        actionQuit = new QAction(SimplePlayer);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionPause = new QAction(SimplePlayer);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        actionPause->setCheckable(true);
        actionStop = new QAction(SimplePlayer);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionOpenLocal = new QAction(SimplePlayer);
        actionOpenLocal->setObjectName(QStringLiteral("actionOpenLocal"));
        actionOpenUrl = new QAction(SimplePlayer);
        actionOpenUrl->setObjectName(QStringLiteral("actionOpenUrl"));
        centralwidget = new QWidget(SimplePlayer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        openLocal = new QPushButton(centralwidget);
        openLocal->setObjectName(QStringLiteral("openLocal"));

        gridLayout_2->addWidget(openLocal, 0, 0, 1, 1);

        openUrl = new QPushButton(centralwidget);
        openUrl->setObjectName(QStringLiteral("openUrl"));

        gridLayout_2->addWidget(openUrl, 0, 1, 1, 1);

        video = new VlcWidgetVideo(centralwidget);
        video->setObjectName(QStringLiteral("video"));

        gridLayout_2->addWidget(video, 1, 0, 1, 2);

        stop = new QPushButton(centralwidget);
        stop->setObjectName(QStringLiteral("stop"));

        gridLayout_2->addWidget(stop, 3, 1, 1, 1);

        pause = new QPushButton(centralwidget);
        pause->setObjectName(QStringLiteral("pause"));
        pause->setCheckable(true);

        gridLayout_2->addWidget(pause, 3, 0, 1, 1);

        volume = new VlcWidgetVolumeSlider(centralwidget);
        volume->setObjectName(QStringLiteral("volume"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(volume->sizePolicy().hasHeightForWidth());
        volume->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(volume, 5, 0, 1, 2);

        seek = new VlcWidgetSeek(centralwidget);
        seek->setObjectName(QStringLiteral("seek"));
        sizePolicy.setHeightForWidth(seek->sizePolicy().hasHeightForWidth());
        seek->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(seek, 6, 0, 1, 2);

        equalizer = new QPushButton(centralwidget);
        equalizer->setObjectName(QStringLiteral("equalizer"));

        gridLayout_2->addWidget(equalizer, 4, 0, 1, 1);

        SimplePlayer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SimplePlayer);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuMedia = new QMenu(menubar);
        menuMedia->setObjectName(QStringLiteral("menuMedia"));
        SimplePlayer->setMenuBar(menubar);
        statusbar = new QStatusBar(SimplePlayer);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        SimplePlayer->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuMedia->menuAction());
        menuFile->addAction(actionOpenLocal);
        menuFile->addAction(actionOpenUrl);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuMedia->addAction(actionPause);
        menuMedia->addAction(actionStop);

        retranslateUi(SimplePlayer);
        QObject::connect(actionQuit, SIGNAL(triggered()), SimplePlayer, SLOT(close()));

        QMetaObject::connectSlotsByName(SimplePlayer);
    } // setupUi

    void retranslateUi(QMainWindow *SimplePlayer)
    {
        SimplePlayer->setWindowTitle(QApplication::translate("SimplePlayer", "Demo Player", 0));
        actionQuit->setText(QApplication::translate("SimplePlayer", "Quit", 0));
        actionPause->setText(QApplication::translate("SimplePlayer", "Pause", 0));
        actionStop->setText(QApplication::translate("SimplePlayer", "Stop", 0));
        actionOpenLocal->setText(QApplication::translate("SimplePlayer", "Open local file", 0));
        actionOpenUrl->setText(QApplication::translate("SimplePlayer", "Open URL", 0));
        openLocal->setText(QApplication::translate("SimplePlayer", "Open local file", 0));
        openUrl->setText(QApplication::translate("SimplePlayer", "Open URL", 0));
        stop->setText(QApplication::translate("SimplePlayer", "Stop", 0));
        pause->setText(QApplication::translate("SimplePlayer", "Pause", 0));
        equalizer->setText(QApplication::translate("SimplePlayer", "Equalizer", 0));
        menuFile->setTitle(QApplication::translate("SimplePlayer", "File", 0));
        menuMedia->setTitle(QApplication::translate("SimplePlayer", "Media", 0));
    } // retranslateUi

};

namespace Ui {
    class SimplePlayer: public Ui_SimplePlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLEPLAYER_H

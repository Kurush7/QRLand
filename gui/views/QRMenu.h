//
// Created by kurush on 17.10.2020.
//

#ifndef BIG3DFLUFFY_QRMENU_H
#define BIG3DFLUFFY_QRMENU_H

#include "QTIncludes.h"
#include <QMenu>
#include <QMenuBar>
#include "QRSettings.h"

class QRMenu: public QMenuBar {
public:
    QRMenu(QWidget *parent= nullptr): QMenuBar(parent) {
        auto *file = addMenu("&Файл");
            auto *open = new QAction("Открыть...", this);
            file->addAction(open);
            auto *save = new QAction("Сохранить", this);
            file->addAction(save);
            auto *saveas = new QAction("Сохранить как...", this);
            file->addAction(saveas);
            auto *quit = new QAction("Выход", this);
            file->addAction(quit);

        auto *settings = addMenu("Правка");
        auto *set_action = new QAction("Параметры", this);
        settings->addAction(set_action);

        auto *view = addMenu("Вид");
        auto *help = addMenu("Справка");

        connect(quit, &QAction::triggered, qApp, QApplication::quit);
        connect(set_action, &QAction::triggered, []() {
            auto w = new QRSettings();
            w->show();
        });
    }
};


#endif //BIG3DFLUFFY_QRMENU_H

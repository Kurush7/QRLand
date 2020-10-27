//
// Created by kurush on 17.10.2020.
//

#include "QRMenu.h"
#include "../prod/MainWindow.h"

QRMenu::QRMenu(sptr<Facade> facade, QWidget *parent): QMenuBar(parent), facade(facade) {
    auto *file = addMenu("&Файл");

    if (facade) {
        auto save = new QAction("Сохранить", this);
        file->addAction(save);
        auto saveas = new QAction("Сохранить в формате STL", this);
        file->addAction(saveas);

        connect(save, &QAction::triggered, [this]() {
            auto file = QFileDialog::getSaveFileName(0, "Сохранить проект",
                                                     "", "*.qrland").toStdString();
            this->facade->save(file, false);
        });
        connect(saveas, &QAction::triggered, [this]() {
            auto file = QFileDialog::getSaveFileName(0, "Сохранить проект",
                                                     "", "*.stl").toStdString();
            this->facade->save(file, true);
        });
    }

    if (facade) {
        auto *quit = new QAction("Закрыть проект", this);
        file->addAction(quit);
        connect(quit, &QAction::triggered, [this]() {
            auto w = new MainWindow();
            w->show();
            emit onQuit();
        });
    }

    auto *exit = new QAction("Выход", this);
    file->addAction(exit);

    auto *settings = addMenu("Правка");
    auto *set_action = new QAction("Параметры", this);
    settings->addAction(set_action);

    auto *view = addMenu("Вид");
    auto *help = addMenu("Справка");

    connect(exit, &QAction::triggered, qApp, QApplication::quit);
    connect(set_action, &QAction::triggered, []() {
        auto w = new QRSettings();
        w->show();
    });

}
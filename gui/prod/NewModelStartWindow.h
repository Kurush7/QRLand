//
// Created by kurush on 21.10.2020.
//

#ifndef BIG3DFLUFFY_NEWMODELSTARTWINDOW_H
#define BIG3DFLUFFY_NEWMODELSTARTWINDOW_H

#include "../views/InputWidget.h"
#include "PlatesWindow.h"
#include "QRLandscape.h"

class NewModelStartWindow: public InputWidget {
public:
    NewModelStartWindow(QWidget *parent = nullptr): InputWidget("Создание нового ландшафта: шаг 1", parent) {
        ui->addLayers("data save", QRVert);

        addInput("начальная длина карты (кол-во точек):", &w, "data");
        addInput("начальная ширина карты (кол-во точек):", &h, "data");
        //addInput("шаг сетки:", &step, "data");
        addInput("зерно генератора случайных значений:", &seed, "data");

        auto next = new QPushButton("далее", this);
        ui->addWidgets({{"save", next}}, "save");
        next->setFixedWidth(128);
        connect(next, &QPushButton::clicked, [this]() {
            w = squaredInc(w);
            h = squaredInc(h);
            auto d = ModelInitData(w, h, seed, step);
            random_seed = seed;
            auto *w = new PlatesWindow(d);
            w->show();
            this->close();
        });

        finish();
    }

private:
    int w = 32, h = 32, seed = 17;
    float step = 1;
};


#endif //BIG3DFLUFFY_NEWMODELSTARTWINDOW_H

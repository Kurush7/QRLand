//
// Created by kurush on 21.10.2020.
//

#ifndef BIG3DFLUFFY_PLATESWINDOW_H
#define BIG3DFLUFFY_PLATESWINDOW_H

#include "../views/InputWidget.h"
#include "QRLandscape.h"
#include "EditorWindow.h"

class PlatesWindow: public InputWidget {
public:
    PlatesWindow(ModelInitData dt, QWidget *parent = nullptr):
    InputWidget("Создание нового ландшафта: шаг 2", parent), data(dt) {
        default_generator = std::default_random_engine(data.seed);
        canvas = sptr<QRCanvas>(new QRCanvas(400, 400, this));
        img = sptr<QRImage>(new ImageQT(canvas));
        initPlatesAndDrawer();

        ui->addLayers("canvas data save", QRVert);
        ui->addWidgets({{"canvas", canvas.get()}}, "canvas");

        addInput("число плит:", &cnt, "data");

        auto next = new QPushButton("далее", this);
        next->setFixedWidth(128);

        auto redo = new QPushButton("обновить", this);
        redo->setFixedWidth(128);

        ui->addLayers("hor", QRHor, "save");
        ui->addWidgets({{"redo", redo}, {"next", next},}, "save/hor");

        connect(redo, &QPushButton::clicked, [this]() {
            initPlatesAndDrawer();
        });

        connect(next, &QPushButton::clicked, [this]() {
            manager->setMove(moves.begin());
            manager->setPlates(plates.begin());
            data.plateManager = manager;

            auto x = new EditorWindow(data);
            x->show();
            this->close();
        });

        finish();
    }

private:
    ModelInitData data;
    int cnt = 4;
    sptr<QRImage> img;
    sptr<QRCanvas> canvas;
    sptr<PlateManager> manager;
    sptr<FigureDrawer> drawer;

    QRVector<sptr<QRFrame2D>> plates;
    QRVector<Vector3D> moves;

    void initPlatesAndDrawer() {
        manager = sptr<PlateManager>(new PlateManager(data.w * data.step,
                                                      data.h * data.step, cnt));
        drawer = sptr<FigureDrawer>(new FigureDrawer(img));
        float kw = img->getWidth() / data.w / data.step;
        float kh = img->getHeight() / data.h / data.step;
        drawer->setScale(kw, kh);
        plates = manager->getPlates();
        moves = manager->getMove();
        for (auto f = plates.begin(); f; f++)
            drawer->addFigure(*f);
        drawer->draw();
    }
};


#endif //BIG3DFLUFFY_PLATESWINDOW_H

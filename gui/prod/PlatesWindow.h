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

        canvas->setToolTip("Вы можете изменять форму, направление и скорость движения литосферных плит:\n"
                           "- кликните по вершине многоугольника и перемещайте ее;\n"
                           "- кликните по концу стрелки и перемещайте ее. \n"
                           "Стрелка обозначает направление"
                           "движения плиты\n, длина пропорциональна скорости движения.");

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

        connect(canvas.get(), &QRCanvas::QRMousePressed,
                [this](float x, float y, QRModifiers m) {onMousePressed(x, y);});
        connect(canvas.get(), &QRCanvas::QRMouseMoved,
                [this](float dx, float dy, QRModifiers m) {onMouseMoved(dx, dy);});
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
    QRVector<sptr<QRArrow2D>> arrows;

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
        arrows.clear();
        for (int i = 0; i < moves.getSize(); ++i) {
            auto inter = plates[i]->rayCenterIntersect(moves[i]);
            float len = vectorLen(plates[i]->center - inter);
            arrows.push_back(sptr<QRArrow2D>(new QRArrow2D(plates[i]->getCenter(),
                                                            plates[i]->getCenter() + moves[i] * len,
                                                            QRColor("red"))));
            drawer->addFigure(arrows[i]);
        }
        drawer->draw();
    }


    int whatPressed = 0; // 0 - nothing, 1 - arrow, 2 - line
    sptr<QRArrow2D> pressedArrow;
    int PAIndex = 0;
    QRVector<QRPair<int, int>> pressedPoints;

    // todo control release
    void onMousePressed(float x, float y) {
        whatPressed = 0;
        Vector3D p{x, y};
        for (int i = 0; i < arrows.getSize(); ++i) {
            Vector3D v = arrows[i]->b;
            v = drawer->transformer->transform(v);
            if (vectorLen2(v - p) < 5) {
                pressedArrow = arrows[i];
                whatPressed = 1;
                PAIndex = i;
                return;
            }
        }

        pressedPoints.clear();
        for (int i = 0; i < plates.getSize(); ++i)
            for (int j = 0; j < plates[i]->points.getSize(); ++j) {
                Vector3D v = plates[i]->points[j];
                v = drawer->transformer->transform(v);
                if (vectorLen2(v - p) < 5) {
                    whatPressed = 2;
                    pressedPoints.push_back({i, j});
                }
            }
    }

    void onMouseMoved(float dx, float dy) {
        Vector3D p{dx, dy};
        p[0] *= data.w * data.step / img->getWidth();
        p[1] *= data.h * data.step / img->getHeight();

        if (!whatPressed) return;
        else if (whatPressed == 1) {
            auto c = plates[PAIndex]->center;
            pressedArrow->b += p;
            moves[PAIndex] = pressedArrow->b - plates[PAIndex]->center;
            auto inter = plates[PAIndex]->rayCenterIntersect(moves[PAIndex]);
            float len = vectorLen2(plates[PAIndex]->center - inter);
            float curlen = vectorLen2(pressedArrow->b - plates[PAIndex]->center);
            if (curlen > len) {
                pressedArrow->b = inter;
                curlen = 1;
            }
            moves[PAIndex] = len2Norm(moves[PAIndex]) * curlen;
            pressedArrow->updateEq();
        }
        else if (whatPressed == 2) {
            for (int i = 0; i < pressedPoints.getSize(); ++i) {
                int pl = pressedPoints[i].fst, pt = pressedPoints[i].snd;
                if (plates[pl]->points[pt][0] == 0) p[0] = 0;
                if (plates[pl]->points[pt][0] == data.w * data.step) p[0] = 0;
                if (plates[pl]->points[pt][1] == 0) p[1] = 0;
                if (plates[pl]->points[pt][1] == data.h * data.step) p[1] = 0;
                plates[pl]->points[pt] += p;
                plates[pl]->updateEdges();

                plates[pl]->updateCenter();
                auto inter = plates[pl]->rayCenterIntersect(moves[pl]);
                float len = vectorLen(plates[pl]->center - inter);
                arrows[pl]->a = plates[pl]->getCenter();
                arrows[pl]->b = plates[pl]->getCenter() + moves[pl] * len;
                arrows[pl]->updateEq();
            }
        }

        drawer->draw();
    }
};


#endif //BIG3DFLUFFY_PLATESWINDOW_H

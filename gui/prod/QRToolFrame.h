//
// Created by kurush on 24.10.2020.
//

#ifndef BIG3DFLUFFY_QRTOOLFRAME_H
#define BIG3DFLUFFY_QRTOOLFRAME_H

#include "2Domain.h"
#include "Facade.h"
#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "landscape/managers/LandscapeBuilder.h"

class QRToolFrame: public QFrame {
public:
    QRToolFrame(QString label, function<void(bool)> exec, function<void(float)> onChanged,
            QWidget *parent=nullptr): QFrame(parent) {
        ui = new QRLayoutManager("global", QRHor);

        auto play = new QPushButton("▶", this);
        auto lab = new QRLabel(label, this);
        slider = new QSlider(Qt::Horizontal , this);
        slider->setMinimum(0);
        slider->setMaximum(100);
        slider->setValue(50);

        ui->addWidgets({{"play", play}, {"label", lab}, {"slider", slider}});

        setLayout(ui->getRootLayout());
        setFrameShape(Box);

        connect(slider, &QSlider::valueChanged, [this, onChanged]() {
            onChanged((float)slider->value() / 100.);
        });

        connect(play, &QPushButton::clicked, [this, play, exec](){
                if (play->text() == "▶") {
                    play->setText("◾");
                    play->setStyleSheet("color:yellow");
                    exec(false);
                }
                else {
                    play->setText("▶");
                    play->setStyleSheet("color:#3873d9");
                    exec(true);
                }
        });

        play->setFixedWidth(32);
        play->setStyleSheet("color:#3873d9");
    }

    QRLayoutManager *ui;
    QSlider *slider;
};


static int riverSourceCnt = 0;
class WaterSourceToolFrame: public QFrame {
Q_OBJECT
public:
    WaterSourceToolFrame(sptr<Facade> facade,
            QLayout *lay, QWidget *parent=nullptr): QFrame(parent),
                                                                 insert_lay(lay), facade(facade) {
        ui = new QRLayoutManager("global", QRHor);

        auto xi = new QRInput("x:", &x);
        auto yi = new QRInput("y:", &y);
        auto submit = new QPushButton("добавить", this);

        ui->addWidgets({{"xl", xi->label}, {"xe", xi->edit},
                        {"yl", yi->label}, {"ye", yi->edit},
                        {"btn", submit}});

        setLayout(ui->getRootLayout());
        setFrameShape(Box);

        connect(submit, &QPushButton::clicked, [this, facade](){
            size_t x0, y0;
            x0 = x / facade->builder->getWorldStep() / miniMapSize * facade->builder->getWidth();
            y0 = facade->builder->getHeight()-1 -
                    y / facade->builder->getWorldStep() / miniMapSize * facade->builder->getHeight();
            x0 = max((size_t)1, min(x0, facade->builder->getHeightMap().width()-2));    // todo a bit of hardcode
            y0 = max((size_t)1, min(y0, facade->builder->getHeightMap().height()-2));
            source_number = facade->builder->waterManager->addRiverSource(x0, y0);

            int sn = source_number;
            auto f = new QRToolFrame("источник: x=" + QString::number(int(x*100)/100.)
                    + ", y=" + QString::number(int(y*100)/100.),[sn, facade](bool x) {
                    facade->builder->waterManager->setSourceEnabled(sn, x);
            },
            [sn, facade](float val) {
                facade->builder->waterManager->setSourceIntensity(sn, val);
            });
            insert_lay->addWidget(f);
            emit deleted();
        });
    }

signals:
    void deleted();

private:
    sptr<Facade> facade;
    QLayout *insert_lay;
    QRLayoutManager *ui;
    float x=0, y=0;
    int source_number = -1;
};


#endif //BIG3DFLUFFY_QRTOOLFRAME_H

//
// Created by kurush on 24.10.2020.
//

#ifndef BIG3DFLUFFY_QRTOOLFRAME_H
#define BIG3DFLUFFY_QRTOOLFRAME_H

#include "2Domain.h"
#include "Facade.h"
#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"

class QRToolFrame: public QWidget {
public:
    QRToolFrame(QRVector<QWidget*> data, QWidget *parent=nullptr): QWidget(parent) {
        ui = new QRLayoutManager("global", QRHor);

        auto play = new QPushButton("◾", this); //▶
        auto submit = new QPushButton("добавить", this);

        ui->addWidgets({{"play", play}});

        int i = 0;
        for (auto &w: data) {
            ui->addWidgets({{(QString("data") + QString::number(i)).toStdString(), w}});
            i++;
        }

        ui->addWidgets({{"btn", submit}});
    }

    QRLayoutManager *ui;
};

class WaterSourceToolFrame: public QFrame {
public:
    WaterSourceToolFrame(QWidget *parent=nullptr): QFrame(parent) {

        auto xi = new QRInput("x:", &x, this);
        auto yi = new QRInput("y:", &y, this);

        frame = new QRToolFrame({xi->label, xi->edit, yi->label, yi->edit}, parent);
        setLayout(frame->ui->getRootLayout());
        QLayout *lay;
        //setStyleSheet("QFrame{border: 2px solid green;}");
        setFrameShape(Box);
    }

private:
    QRToolFrame *frame;

    float x=0, y=0;
};


#endif //BIG3DFLUFFY_QRTOOLFRAME_H

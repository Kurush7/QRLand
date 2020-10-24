//
// Created by kurush on 24.10.2020.
//

#ifndef BIG3DFLUFFY_EDITORWATERWIDGET_H
#define BIG3DFLUFFY_EDITORWATERWIDGET_H

#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>

#include "2Domain.h"
#include "Facade.h"
#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "QRToolFrame.h"


class EditorWaterWidget: public QWidget {
public:
    EditorWaterWidget(sptr<Facade> f, QWidget *parent=nullptr): QWidget(parent), facade(f) {
        ui = new QRLayoutManager("global", QRVert);

        auto show = new QCheckBox("отображать воду", this);
        connect(show, &QCheckBox::clicked, [this, show]() {facade->setWaterVisible(show->isChecked());});
        show->setChecked(true);

        auto setWaterLevel = new QRInputBtn("уровень воды:", "установить", &waterLevel, this,
                [this](){facade->builder->waterManager->setWaterLevel(waterLevel);});

        ui->addLayers("check setLevel $ data $", QRHor);
        ui->addWidgets({{"show", show}}, "check");
        ui->addWidgets({{"input", setWaterLevel}}, "setLevel");

        auto dt = new WaterSourceToolFrame(this);
        ui->addWidgets({{"w", dt}}, "data");


        ui->generateSpacers();
        setLayout(ui->getRootLayout());
    }

private:
    QRLayoutManager *ui;
    sptr<Facade> facade;
    float waterLevel;
};


#endif //BIG3DFLUFFY_EDITORWATERWIDGET_H

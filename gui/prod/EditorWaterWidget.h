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
#include "ActionManager.h"

class EditorWaterWidget: public QWidget {
public:
    EditorWaterWidget(sptr<Facade> f, sptr<ActionManager>, QWidget *parent=nullptr);


    QPushButton *iterate;
    QTimer *erosionTimer;
private:
    bool adding = false;
    sptr<ActionManager> manager;
    QRLayoutManager *ui;
    sptr<Facade> facade;
    float waterLevel;

    QWidget *w;
    QScrollArea *scr;
    QVBoxLayout *lay;
    WaterSourceToolFrame *addWidget = nullptr;
    int rain_source;

    void addSource() {
        if (adding) return;
        addWidget = new WaterSourceToolFrame(facade, lay, this);
        lay->addWidget(addWidget);
        adding = true;
        connect(addWidget, &WaterSourceToolFrame::deleted, [this](){
            adding = false;
            delete addWidget; addWidget=nullptr;});
    }

    void addRainSource(int init = -1) {
        int rain_source = init;
        if (rain_source == -1)
            rain_source = facade->builder->waterManager->addRainSource();

        auto f = new QRToolFrame("дождь", [this, rain_source](bool x) {
            facade->builder->waterManager->setSourceEnabled(rain_source, x);
        },
 [this, rain_source](float val) {
             facade->builder->waterManager->setSourceIntensity(rain_source, val);
         });
        lay->addWidget(f);
    }

    void addRiverSource(int init = -1, Vector3D data=ZeroVector) {
        // todo check lambda correct capturing
        int rain_source = init;
        if (rain_source == -1)
            rain_source = facade->builder->waterManager->addRainSource();

        QString label = "источник: x=" + QString::number(int(data[0]*10)/10.)
                + ", y=" + QString::number(int(data[1]*10)/10.);
        auto f = new QRToolFrame(label, [this, rain_source](bool x) {
                                     facade->builder->waterManager->setSourceEnabled(rain_source, x);
                                 },
                                 [this, rain_source](float val) {
                                     facade->builder->waterManager->setSourceIntensity(rain_source, val);
                                 });
        lay->addWidget(f);
    }


    void loadSources() {
        int i = 0;
        for (auto &s: facade->builder->waterManager->waterSources) {
            Vector3D data = s->getData();
            float step = facade->builder->getWorldStep();

            data[0] = data[0] / facade->builder->getWidth() * miniMapSize;
            data[1] = data[1] / facade->builder->getHeight() * miniMapSize;

            if (data != ZeroVector) {
                data[1] = miniMapSize - data[1];
                addRiverSource(i+1, data);
            }
            i++;
        }
    }
};

#endif //BIG3DFLUFFY_EDITORWATERWIDGET_H

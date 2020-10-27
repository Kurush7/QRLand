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

    void addRainSource() {
        rain_source = facade->builder->waterManager->addRainSource();
        auto f = new QRToolFrame("дождь", [this](bool x) {
            facade->builder->waterManager->setSourceEnabled(rain_source, x);
        },
 [this](float val) {
             facade->builder->waterManager->setSourceIntensity(rain_source, val);
         });
        lay->addWidget(f);
    }
};

#endif //BIG3DFLUFFY_EDITORWATERWIDGET_H

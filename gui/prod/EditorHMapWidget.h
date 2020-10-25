//
// Created by kurush on 25.10.2020.
//

#ifndef STYLE_CSS_EDITORHMAPWIDGET_H
#define STYLE_CSS_EDITORHMAPWIDGET_H

#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>

#include "2Domain.h"
#include "Facade.h"
#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "QRToolFrame.h"

const int hmapFPS = 2;  // todo fuck off from here

class EditorHMapWidget: public QWidget {
public:
    EditorHMapWidget(sptr<Facade> f, QWidget *parent=nullptr);

private:
    //bool adding = false;
    QRLayoutManager *ui;
    sptr<Facade> facade;
    //float waterLevel;

    QWidget *w;
    QScrollArea *scr;
    QVBoxLayout *lay;
    //WaterSourceToolFrame *addWidget = nullptr;
    int mountain_source, hill_source;

    void addMountainSource() {
        mountain_source = facade->builder->addTool({PlateMountainsTool, freqUNIQUE});
        auto f = new QRToolFrame("горы", [this](bool x) {
                                     facade->builder->disturbManager.setActive(mountain_source, x);
                                 },
                                 [this](float val) {
                                     facade->builder->disturbManager.setIntensity(mountain_source, val);
                                 });
        lay->addWidget(f);
    }

    void addHillSource() {
        hill_source = facade->builder->addTool({HillSetTool, freqUNIQUE});
        auto f = new QRToolFrame("холмы", [this](bool x) {
                                     facade->builder->disturbManager.setActive(hill_source, x);
                                 },
                                 [this](float val) {
                                     facade->builder->disturbManager.setIntensity(hill_source, val);
                                 });
        lay->addWidget(f);
    }
};


#endif //STYLE_CSS_EDITORHMAPWIDGET_H

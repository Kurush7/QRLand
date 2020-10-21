//
// Created by kurush on 21.10.2020.
//

#ifndef BIG3DFLUFFY_INPUTWIDGET_H
#define BIG3DFLUFFY_INPUTWIDGET_H

#include "QTIncludes.h"
#include "../widgets/layoutManager/QRLayoutManager.h"
#include "../widgets/QRWidgets.h"
#include "QRConstants.h"
#include "configManager.h"

class InputWidget: public QWidget {
public:
    InputWidget(std::string title, QWidget *parent=nullptr): QWidget(parent) {
        setWindowTitle(title.c_str());
        ui = new QRLayoutManager("global", QRVert);
    }
protected:
    void finish() {
        ui->generateSpacers();
        setLayout(ui->getRootLayout());
    }

    QRLayoutManager *ui;

    void addInput(std::string name, float* var, std::string where, bool reset=false) {
        static int cnt=0;
        if (reset) cnt=0;
        std::string l = "input" + std::to_string(cnt);
        ui->addWidgets({{l, new QRInput(name, var, this)}}, where);
    }
    void addInput(std::string name, int* var, std::string where, bool reset=false) {
        static int cnt=0;
        if (reset) cnt=0;
        std::string l = "input" + std::to_string(cnt);
        ui->addWidgets({{l, new QRInput(name, var, this)}}, where);
    }
};

#endif //BIG3DFLUFFY_INPUTWIDGET_H

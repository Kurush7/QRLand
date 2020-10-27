//
// Created by kurush on 25.10.2020.
//

#ifndef STYLE_CSS_EDITORMISCWIDGET_H
#define STYLE_CSS_EDITORMISCWIDGET_H

#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>

#include "2Domain.h"
#include "Facade.h"
#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "QRToolFrame.h"

class EditorMiscWidget: public QWidget {
public:
    EditorMiscWidget(sptr<Facade> f, QWidget *parent=nullptr);

private:
    float x=1, y=1, z=-0.5;
    QRLayoutManager *ui;
    sptr<Facade> facade;

};



#endif //STYLE_CSS_EDITORMISCWIDGET_H

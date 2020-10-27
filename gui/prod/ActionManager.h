//
// Created by kurush on 27.10.2020.
//

#ifndef STYLE_CSS_ACTIONMANAGER_H
#define STYLE_CSS_ACTIONMANAGER_H


#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>
#include <thread>

#include "2Domain.h"
#include "Facade.h"
#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "QRToolFrame.h"

//builder->climateManager->on_the_7th_day();
//builder->waterManager->updateWater();

//    landscape->interpolateColors();
//topDown->drawMiniMap();


class ActionManager: public QObject {
    Q_OBJECT
public:
    ActionManager(sptr<Facade> facade): facade(facade) {}

    void processHMap() {
        if (!capture()) {
            queue.push(0);
            return;
        }
        hmap_cnt++;
        facade->process(false);
        //facade->erosionIteration(false, false);
        if (hmap_cnt == hmapLimit) {
            hmap_cnt=0;
            queue.push(2);
        }
        release();
    }
    void processWater(bool forceDraw=false) {
        if (!capture()) {
            queue.push(1);
            return;
        }
        water_cnt++;
        facade->erosionIteration(false, !forceDraw);
        if (water_cnt == waterLimit || forceDraw) {
            water_cnt=0;
            facade->updateClimate(true);
            facade->topDown->drawMiniMap();
        }
        release();
    }

signals:
    void finished();

private:
    int hmapLimit=2, waterLimit=30;
    int hmap_cnt=0, water_cnt=0;

    bool buzy = false;
    sptr<Facade> facade;
    std::mutex buzy_mutex;
    QRQueue<int> queue; // 0 - hmap, 1 - water, 2 - water from hmap;

    void next() {
        if (queue.isEmpty()) return;
        int x = queue.pop();
        if (x == 0)
            processHMap();
        else processWater(x==2);
    }

    bool capture() {
        bool flag = true;

        buzy_mutex.lock();
        if (!buzy) buzy = true;
        else flag = false;
        buzy_mutex.unlock();

        return flag;
    }
    void release() {
        buzy_mutex.lock();
        buzy = false;
        buzy_mutex.unlock();
        next();
    }
};


#endif //STYLE_CSS_ACTIONMANAGER_H

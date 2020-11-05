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

#include "EditorPresenter.h"

//builder->climateManager->on_the_7th_day();
//builder->waterManager->updateWater();

//    landscape->interpolateColors();
//topDown->drawMiniMap();


class ActionManager: public QObject {
    Q_OBJECT
public:
    ActionManager(sptr<Facade> facade, EditorPresenter *presenter)
    : presenter(presenter), facade(facade) {}

    void processHMap() {
        if (!capture()) {
            queue.push(0);
            return;
        }
        hmap_cnt++;
        facade->process(false);
        facade->updateClimate(hmap_cnt == hmapLimit);
        if (hmap_cnt == hmapLimit) {
            hmap_cnt=0;
        }
        release();
    }

    void processWater(bool forceDraw=false) {
        if (!capture()) {
            queue.push(1);
            return;
        }
        water_cnt++;
        // todo check for rescale bugs
        facade->erosionIteration(false, true);
        if (water_cnt == waterLimit || forceDraw) {
            water_cnt=0;
            facade->updateClimate(true);
            facade->topDown->drawMiniMap();
        }
        release();
    }

    void move(float _dx, float _dy, float _dz) {
        dx.push(_dx), dy.push(_dy), dz.push(_dz);
        actionType.push(0);
        actionIsLife();
    }

    void scale(float _dx, float _dy, float _dz) {
        dx.push(_dx), dy.push(_dy), dz.push(_dz);
        actionType.push(1);
        actionIsLife();
    }

    void rotate(float _dx, float _dy, float _dz) {
        dx.push(_dx), dy.push(_dy), dz.push(_dz);
        actionType.push(2);
        actionIsLife();
    }

    void actionIsLife() {
        if (!capture()) {
            queue.push(3);
            return;
        }
        float x = dx.pop(), y = dy.pop(), z = dz.pop();
        int type = actionType.pop();
        if (type == 0) facade->moveCamera(x, y, z);
        else if (type == 1) facade->scaleCamera(x, y, z);
        else if (type == 2) facade->rotateCamera(x, y, z);
        presenter->draw();
        release();
    }

signals:
    void finished();

private:
    EditorPresenter *presenter;

    QRQueue<float> dx, dy, dz;
    QRQueue<int> actionType; // 0 - move, 1 - scale, 2 - rotate

    int hmapLimit=2, waterLimit=1;//30;
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
        else if (x == 1 || x == 2)
            processWater(x==2);
        else if (x == 3)
            actionIsLife();
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

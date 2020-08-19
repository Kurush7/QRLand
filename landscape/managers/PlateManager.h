//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_PLATEMANAGER_H
#define BIG3DFLUFFY_PLATEMANAGER_H

#include <random>

#include "2Domain.h"
#include "../landscapeConfig.h"

class PlateManager {
public:
    PlateManager(double width, double height,
            size_t plateCnt=defaultPlateCnt): w(width), h(height) {
        /*QRVector<Vector3D> plate;
        plate.push_back({0,0,0});
        plate.push_back({5,0,0});
        plate.push_back({5,5,0});
        plate.push_back({0,5,0});
        plates.push_back(sptr<QRFrame2D>(new QRFrame2D(plate,
                QRColor("red"))));*/
        buildPlates(definePoints(plateCnt));
    }

    QRVectorIterator<sptr<QRFrame2D>> getPlates() {return plates.begin();}

private:
    QRVector<Vector3D> definePoints(int cnt);
    void buildPlates(QRVector<Vector3D> points);

    double w, h;
    QRVector<sptr<QRFrame2D>> plates;

    std::default_random_engine generator = std::default_random_engine();
};


#endif //BIG3DFLUFFY_PLATEMANAGER_H

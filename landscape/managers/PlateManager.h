//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_PLATEMANAGER_H
#define BIG3DFLUFFY_PLATEMANAGER_H

#include <random>

#include "2Domain.h"
#include "QRConstants.h"
#include "random_generator.h"

class PlateManager {
public:
    PlateManager(double width, double height,
            size_t plateCnt=defaultPlateCnt): w(width), h(height) {
        buildPlates(definePoints(plateCnt));
    }

    void scalePlates() {
        for (auto &p: plates)
            for (auto &e: p->edges) {
                e.a[0] *= 2, e.a[1] *= 2;
                e.b[0] *= 2, e.b[1] *= 2;
            }
    }

    QRVectorIterator<sptr<QRFrame2D>> getPlates() {return plates.begin();}
    QRVectorIterator<Vector3D> getMove() {return moveVectors.begin();}

    void setPlates(QRVectorIterator<sptr<QRFrame2D>> ps) {plates = ps;}
    void setMove(QRVectorIterator<Vector3D> mv) {moveVectors = mv;}

private:
    QRVector<Vector3D> definePoints(int cnt);
    void buildPlates(QRVector<Vector3D> points);

    double w, h;
    QRVector<sptr<QRFrame2D>> plates;
    QRVector<Vector3D> moveVectors;
};


#endif //BIG3DFLUFFY_PLATEMANAGER_H

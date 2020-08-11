//
// Created by kurush on 11.08.2020.
//

#ifndef BIG3DFLUFFY_ROAMLANDSCAPE_H
#define BIG3DFLUFFY_ROAMLANDSCAPE_H

#include "../QRLandscapeSurface.h"
#include "RoamTree.h"

class RoamLandscape: public QRPolyModel3D {
public:
    // points must be square matrix with size 2^n+1
    RoamLandscape(const QRMatrix<sptr<QRPoint3D>> &points);
    ~RoamLandscape() {
        delete[] frames;
    }

    virtual void updateCamera(const sptr<QRCamera3D> &camera);
    virtual bool isAdditivePolygons() {return true;}
    void addPolygons(QRVector<sptr<QRPolygon3D>> &polygons);

    virtual PolygonIterator getPolygons() const { return PolygonIterator();}
    virtual PointIterator getPoints() const {return points.begin();} // todo not working
    virtual sptr<QRPolygon3D>* getPurePolygons() const {return nullptr;} // todo not working
    virtual sptr<QRPoint3D>* getPurePoints() const {return points.getPureArray();}
    virtual size_t getPolygonCnt() const {return lastPolyCount;}
    virtual size_t getPointCnt() const {return points.getSize();}


    virtual void setValues(PointIterator pt, PolygonIterator pg) {}       //todo
    virtual float getRadius() const {return QRINF;}                 // todo
    virtual void setSelected(bool x) {}     // todo

private:
    LinkMap links;
    size_t dimFrameCnt;
    size_t lastPolyCount = 0;
    QRMatrix<sptr<QRPoint3D>> points;
    Frame *frames;
};


#endif //BIG3DFLUFFY_ROAMLANDSCAPE_H

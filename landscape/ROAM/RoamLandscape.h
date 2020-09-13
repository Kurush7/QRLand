//
// Created by kurush on 11.08.2020.
//

#ifndef BIG3DFLUFFY_ROAMLANDSCAPE_H
#define BIG3DFLUFFY_ROAMLANDSCAPE_H

#include "../basic/QRLandscapeSurface.h"
#include "RoamTree.h"

class RoamLandscape: public QRPolyModel3D {
public:
    // points must be square matrix with size 2^n+1
    explicit RoamLandscape(const QRMatrix<sptr<QRPoint3D>> &points);
    ~RoamLandscape() {
        delete[] frames;
    }

    virtual bool isAdditivePolygons() {return true;}
    virtual void updateCamera(const sptr<QRCamera3D> &camera, std::string *info=nullptr);
    virtual void addPolygons(QRVector<sptr<QRPolygon3D>> &polygons, std::string *info=nullptr);
    virtual void addAllPolygons(QRVector<sptr<QRPolygon3D>> &polygons, std::string *info=nullptr);
    virtual void addMaxDetailPolygons(QRVector<sptr<QRPolygon3D>> &polygons, std::string *info=nullptr);

    virtual PolygonIterator getPolygons() const;
    virtual PointIterator getPoints() const {return points.begin();}
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
    size_t lowestPolygonCnt = 0;
    const QRMatrix<sptr<QRPoint3D>> points;
    Frame *frames;
};


#endif //BIG3DFLUFFY_ROAMLANDSCAPE_H

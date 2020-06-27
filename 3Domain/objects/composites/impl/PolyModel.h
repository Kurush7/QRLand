//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_POLYMODEL_H
#define BIG3DFLUFFY_POLYMODEL_H

#include "../interfaces/QRPolyModel3D.h"

class PolyModel: public QRPolyModel3D {
public:
    PolyModel(const PointIterator &pt, const PolygonIterator &pg) {points = pt, polygons = pg;}

    virtual const PolygonIterator& getPolygons() const {return polygons.begin();}
    virtual const PointIterator& getPoints() const {return points.begin();}
    virtual void setValues(const PointIterator &pt, const PolygonIterator &pg) {points = pt, polygons = pg;}
    virtual void setSelected(bool x) {
        for (auto p: points)
            p->setSelected(x);
        for (auto p: polygons)
            p->setSelected(x);
    }

private:
    QRVector<sptr<QRPoint3D>> points;
    QRVector<sptr<QRPolygon3D>> polygons;
};


#endif //BIG3DFLUFFY_POLYMODEL_H

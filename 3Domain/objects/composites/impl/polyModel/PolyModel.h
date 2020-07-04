//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_POLYMODEL_H
#define BIG3DFLUFFY_POLYMODEL_H

#include "../../interfaces/QRPolyModel3D.h"

/**
 model is not binded to it's world position, all coordinates are counted from (0,0,0) point,
 where model's center is considered to be (radius is calculates as distance from (0,0,0) to farest point).
 in order to use/transform the model, you need to keep along with it a transformer/matrix, which
 holds info about model's scene position, rotations etc.
 */
class PolyModel: public QRPolyModel3D {
public:
    PolyModel(const PointIterator &pt, const PolygonIterator &pg) {setValues(pt, pg);}

    virtual PolygonIterator getPolygons() const {return polygons.begin();}
    virtual PointIterator getPoints() const {return points.begin();}
    virtual void setValues(PointIterator pt, PolygonIterator pg);
    virtual void setSelected(bool x) {
        for (auto p: points)
            p->setSelected(x);
        for (auto p: polygons)
            p->setSelected(x);
    }

    virtual float getRadius() const {return radius;}

private:
    QRVector<sptr<QRPoint3D>> points;
    QRVector<sptr<QRPolygon3D>> polygons;
    float radius = 0;

    void defineRadius();
};


#endif //BIG3DFLUFFY_POLYMODEL_H

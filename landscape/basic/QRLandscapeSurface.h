//
// Created by kurush on 29.07.2020.
//

#ifndef BIG3DFLUFFY_QRLANDSCAPESURFACE_H
#define BIG3DFLUFFY_QRLANDSCAPESURFACE_H

#include <initializer_list>
#include "containers/QRContainers.h"
#include "objects/QRObject.h"
#include "objects/composites/impl.h"
#include "objects/components/impl.h"
#include "QRLandscapeTexture.h"

#include "globalConfig.h"

class QRLandscapeSurface: public QRPolyModel3D {
public:
    QRLandscapeSurface(const QRMatrix<sptr<QRPoint3D>> &points);

    virtual PolygonIterator getPolygons() const {return polygons.begin();}
    virtual PointIterator getPoints() const {return points.begin();}
    virtual sptr<QRPolygon3D>* getPurePolygons() const {return polygons.getPureArray();}
    virtual sptr<QRPoint3D>* getPurePoints() const {return points.getPureArray();}
    virtual size_t getPolygonCnt() const {return polygons.getSize();}
    virtual size_t getPointCnt() const {return points.getSize();}


    virtual void setValues(PointIterator pt, PolygonIterator pg) {}       //todo
    virtual float getRadius() const {       // todo fucking wrong
        return vectorLen(points[0][0]->getVector());
    }
    virtual void setSelected(bool x) {}    // todo


private:
    QRMatrix<sptr<QRPoint3D>> points;
    QRVector<sptr<QRPolygon3D>> polygons;
    size_t width, height;
};


#endif //BIG3DFLUFFY_QRLANDSCAPESURFACE_H

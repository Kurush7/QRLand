//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_POLYMODELCREATOR_H
#define BIG3DFLUFFY_POLYMODELCREATOR_H

#include "../composites/interfaces/QRPolyModel3D.h"
#include "../composites/impl/PolyModel.h"
#include "../components/impl/Point3D.h"
#include "../components/impl/Polygon3D.h"

class QRPolyModelCreator {
public:
    virtual bool createPoints() = 0;
    virtual bool createPolygons() = 0;

protected:
    virtual sptr<QRPolyModel3D> getModel() = 0;
    sptr<QRPolyModel3D> model;
};


class PrimitivePolyModelCreator: public QRPolyModelCreator {
protected:
    QRVector<sptr<QRPoint3D>> points;
    QRVector<sptr<QRPolygon3D>> polygons;
};

class CubeModelCreator: public PrimitivePolyModelCreator {
public:
    virtual bool createPoints();
    virtual bool createPolygons();

    sptr<QRPolyModel3D> create(Vector3D _pos, double _a, sptr<QRTexture> txt = nullptr) {
        pos = _pos, a = _a;
        if (txt) texture = txt;
        if (createPoints() && createPolygons()) return getModel();
        return nullptr;
    }

protected:
    virtual sptr<QRPolyModel3D> getModel();
private:
    sptr<QRTexture> texture = DEFAULT_TEXTURE;
    double a;
    Vector3D pos;
};

#endif //BIG3DFLUFFY_POLYMODELCREATOR_H

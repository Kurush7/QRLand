//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYSCENE3D_H
#define BIG3DFLUFFY_QRPOLYSCENE3D_H

#include "../../components/interfaces/QRCamera3D.h"
#include "../../managers/SelectionManager.h"
#include "../../composites/interfaces/QRPolyModel3D.h"
#include "objects/lights/light.h"

/**
 todo
 only one active camera is supported. no selections, no camera switching
 */

using ItemsMap = std::map<int, sptr<QRPolyModel3D>>;
using RawModel = QRPair<sptr<QRPolyModel3D>, sptr<QRTransformer3D>>;
using RawModelIterator = QRVectorIterator<QRPair<sptr<QRPolyModel3D>, sptr<QRTransformer3D>>>;

class QRPolyScene3D {
public:
    QRPolyScene3D() {
        p = sptr<QRPolyScene3D>(this, [](void *ptr){});}
    ~QRPolyScene3D() {p.reset();}
    virtual sptr<QRPolyScene3D> getPointer() {return p;}
    virtual uptr<QRMemento> save();

    // todo maybe transform matrix as unnecessary parameter
    virtual bool addModel(const sptr<QRPolyModel3D> &m, const Vector3D &v) = 0;

    virtual sptr<QRCamera3D> getActiveCamera() const = 0;
    virtual void setActiveCamera(const sptr<QRCamera3D>&) = 0;

    //virtual bool operator==(const QRPolyScene3D &b) const;
    //virtual QRPolyScene3D& operator=(const QRPolyScene3D &p);
    //virtual void deleteModel(const ModelIterator &obj) = 0;
    virtual RawModelIterator getModels() const = 0;
    virtual void setModels(RawModelIterator) = 0;

    virtual void addLight(const sptr<QRLight> &ligth) = 0;

private:
    sptr<QRPolyScene3D> p;
};



#endif //BIG3DFLUFFY_QRPOLYSCENE3D_H

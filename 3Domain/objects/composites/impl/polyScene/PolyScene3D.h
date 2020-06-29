//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_POLYSCENE3D_H
#define BIG3DFLUFFY_POLYSCENE3D_H

#include "../../interfaces/QRPolyScene3D.h"

class PolyScene3D: public QRPolyScene3D {
public:
    virtual bool addModel(const sptr<QRPolyModel3D> &m, const Vector3D &v);

    virtual sptr<QRCamera3D> getActiveCamera() const {return camera;}
    virtual void setActiveCamera(const sptr<QRCamera3D> &c) {camera = c;}

    virtual RawModelIterator getModels() const {return models.begin();}
    virtual void setModels(RawModelIterator it) {models = it;}

    // todo
    //virtual void deleteModel(const ModelIterator &it) {models.pop(it);}
    //virtual bool operator==(const QRPolyScene3D &b) const;
    //virtual QRPolyScene3D& operator=(const QRPolyScene3D &p);

private:
    QRVector<RawModel> models;
    sptr<QRCamera3D> camera;
    // todo lights
};

#endif //BIG3DFLUFFY_POLYSCENE3D_H

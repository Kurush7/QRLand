//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_POLYSCENE3D_H
#define BIG3DFLUFFY_POLYSCENE3D_H

#include "../interfaces/QRPolyScene3D.h"

class PolyScene3D: public QRPolyScene3D {
public:
    virtual bool addModel(const sptr<QRPolyModel3D> &m) {models.push_back(m);}
    virtual void deleteModel(const ModelIterator &it) {models.pop(it);}

    virtual const ModelIterator& getModels() const {return models.begin();}
    virtual void setModels(const ModelIterator &it) {models = it;}
    virtual sptr<QRCamera3D> getActiveCamera() const {return camera;}
    virtual void setActiveCamera(const sptr<QRCamera3D> &c) {camera = c;}

    // todo
    //virtual bool operator==(const QRPolyScene3D &b) const;
    //virtual QRPolyScene3D& operator=(const QRPolyScene3D &p);
private:
    QRVector<sptr<QRPolyModel3D>> models;
    sptr<QRCamera3D> camera;
};


#endif //BIG3DFLUFFY_POLYSCENE3D_H

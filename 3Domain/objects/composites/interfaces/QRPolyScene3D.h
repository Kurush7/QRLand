//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYSCENE3D_H
#define BIG3DFLUFFY_QRPOLYSCENE3D_H

#include "../../components/interfaces/QRCamera3D.h"
#include "../../managers/SelectionManager.h"
#include "../../composites/interfaces/QRPolyModel3D.h"

/**
 todo
 only one active camera is supported. no selections, no camera switching
 */
class QRPolyScene3D {
public:
    QRPolyScene3D() {
        p = sptr<QRPolyScene3D>(this, [](void *ptr){});}
    ~QRPolyScene3D() {p.reset();}
    virtual sptr<QRPolyScene3D> getPointer() {return p;}
    virtual uptr<QRMemento> save();     // todo add new memento!!!!!

    // todo add selections
    //virtual QRVector<sptr<QRObject>> getSelection() = 0;
    //virtual void switchSelection(const ObjectIterator &it) = 0;
    //virtual bool isEmptySelection() = 0;

    virtual bool addModel(const sptr<QRPolyModel3D> &p) = 0;
    virtual void deleteModel(const ModelIterator &obj) = 0;

    virtual const ModelIterator& getModels() const = 0;
    virtual void setModels(const ModelIterator&) = 0;
    virtual sptr<QRCamera3D> getActiveCamera() const = 0;
    virtual void setActiveCamera(const sptr<QRCamera3D>&) = 0;

    // todo
    //virtual bool operator==(const QRPolyScene3D &b) const;
    //virtual QRPolyScene3D& operator=(const QRPolyScene3D &p);

private:
    sptr<QRPolyScene3D> p;
};



#endif //BIG3DFLUFFY_QRPOLYSCENE3D_H

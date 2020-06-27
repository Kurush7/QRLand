//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRSCENE3D_H
#define BIG3DFLUFFY_QRSCENE3D_H

#include "../../components/interfaces/QRCamera3D.h"
#include "../../managers/SelectionManager.h"

// SCENE IS NOT COMPOSITE! IT'S JUST HERE FOR COMFORTABLE ACCESS

class SceneMemento;
class QRScene3D {
public:
    QRScene3D() {
        p = sptr<QRScene3D>(this, [](void *ptr){});}
    ~QRScene3D() {p.reset();}

    virtual QRVector<sptr<QRObject>> getSelection() = 0;
    virtual void switchSelection(const ObjectIterator &it) = 0;
    virtual bool isEmptySelection() = 0;

    virtual bool addObject(sptr<QRObject> obj) = 0;
    virtual void deleteObject(ObjectIterator obj) = 0;

    virtual sptr<QRScene3D> getPointer() {return p;}

    virtual const QRVector<sptr<QRObject>>& getObjects() = 0;
    virtual void setObjects(QRVector<sptr<QRObject>>&) = 0;
    virtual sptr<QRCamera3D> getActiveCamera() = 0;

    virtual uptr<QRMemento> save();

    virtual bool operator==(const QRScene3D &b) const = delete;
    virtual QRScene3D& operator=(const QRScene3D &p) = delete;

private:
    sptr<QRScene3D> p;
};

#endif //BIG3DFLUFFY_QRSCENE3D_H

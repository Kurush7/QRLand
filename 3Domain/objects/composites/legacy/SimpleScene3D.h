//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENE_H
#define BIG3DFLUFFY_SCENE_H

#include "../interfaces/QRScene3D.h"

class SimpleScene3D: public QRScene3D {
public:
    // todo passing value, std::move
    explicit SimpleScene3D(const QRVector<sptr<QRObject>> &obj, sptr<QRCamera3D> cam)
    : objects(obj), activeCamera(cam),
    selectionManager(sptr<BaseSelectionManager>(new SelectionManager(objects))) {}

    explicit SimpleScene3D(sptr<QRCamera3D> cam)
    :activeCamera(cam),
     selectionManager(sptr<BaseSelectionManager>(new SelectionManager(objects))){}

    virtual bool addObject(sptr<QRObject> obj) {objects.push_back(obj);}
    virtual void deleteObject(ObjectIterator obj) {
        if (selectionManager->isSelected(obj))
            selectionManager->switchSelection(obj);
        objects.pop(obj);
    }

    virtual void switchSelection(const ObjectIterator &it) {
        selectionManager->switchSelection(it);
    }
    virtual QRVector<sptr<QRObject>> getSelection() {
        return selectionManager->getSelection();
    }
    virtual bool isEmptySelection() {return selectionManager->isEmptySelection();}

    virtual const QRVector<sptr<QRObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<sptr<QRObject>> &obj) { objects = obj;}
    virtual sptr<QRCamera3D> getActiveCamera() {return activeCamera;}

private:
    QRVector<sptr<QRObject>> objects;
    sptr<QRCamera3D> activeCamera;
    sptr<BaseSelectionManager> selectionManager;
};


#endif //BIG3DFLUFFY_SCENE_H

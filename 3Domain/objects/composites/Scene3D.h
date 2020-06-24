//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENE_H
#define BIG3DFLUFFY_SCENE_H

#include "BaseComposite.h"
#include "../components/interfaces/QRCamera3D.h"
#include "../managers/SelectionManager.h"

// SCENE IS NOT COMPOSITE!

class SceneMemento;
class BaseScene3D {
public:
    BaseScene3D() {
        p = sptr<BaseScene3D>(this, [](void *ptr){});}
    ~BaseScene3D() {p.reset();}

    virtual QRVector<sptr<QRObject>> getSelection() = 0;
    virtual void switchSelection(const ObjectIterator &it) = 0;
    virtual bool isEmptySelection() = 0;

    virtual bool addObject(sptr<QRObject> obj) = 0;
    virtual void deleteObject(ObjectIterator obj) = 0;

    virtual sptr<BaseScene3D> getPointer() {return p;}

    virtual const QRVector<sptr<QRObject>>& getObjects() = 0;
    virtual void setObjects(QRVector<sptr<QRObject>>&) = 0;
    virtual sptr<QRCamera3D> getActiveCamera() = 0;

    virtual uptr<QRMemento> save();

    virtual bool operator==(const BaseScene3D &b) const = delete;
    virtual BaseScene3D& operator=(const BaseScene3D &p) = delete;

private:
    sptr<BaseScene3D> p;
};

class SceneMemento: public QRMemento {
public:
    explicit SceneMemento(sptr<BaseScene3D> scene);
    virtual void restore();
private:
    wptr<BaseScene3D> scene;   // todo camera is keeped, but not restored, since there is no setCamera method
    wptr<QRCamera3D> camera;
    QRVector<sptr<QRObject>> objects;
};

class Scene3D: public BaseScene3D {
public:
    // todo passing value, std::move
    explicit Scene3D(const QRVector<sptr<QRObject>> &obj, sptr<QRCamera3D> cam)
    : objects(obj), activeCamera(cam),
    selectionManager(sptr<BaseSelectionManager>(new SelectionManager(objects))) {}

    explicit Scene3D(sptr<QRCamera3D> cam)
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

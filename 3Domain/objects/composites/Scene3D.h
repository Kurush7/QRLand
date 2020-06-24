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
        p = std::shared_ptr<BaseScene3D>(this, [](void *ptr){});}
    ~BaseScene3D() {p.reset();}

    virtual QRVector<shared_ptr<QRObject>> getSelection() = 0;
    virtual void switchSelection(const ObjectIterator &it) = 0;
    virtual bool isEmptySelection() = 0;

    virtual bool addObject(std::shared_ptr<QRObject> obj) = 0;
    virtual void deleteObject(ObjectIterator obj) = 0;

    virtual std::shared_ptr<BaseScene3D> getPointer() {return p;}

    virtual const QRVector<std::shared_ptr<QRObject>>& getObjects() = 0;
    virtual void setObjects(QRVector<std::shared_ptr<QRObject>>&) = 0;
    virtual std::shared_ptr<QRCamera3D> getActiveCamera() = 0;

    virtual std::unique_ptr<QRMemento> save();

    virtual bool operator==(const BaseScene3D &b) const = delete;
    virtual BaseScene3D& operator=(const BaseScene3D &p) = delete;

private:
    std::shared_ptr<BaseScene3D> p;
};

class SceneMemento: public QRMemento {
public:
    explicit SceneMemento(std::shared_ptr<BaseScene3D> scene);
    virtual void restore();
private:
    std::weak_ptr<BaseScene3D> scene;   // todo camera is keeped, but not restored, since there is no setCamera method
    std::weak_ptr<QRCamera3D> camera;
    QRVector<std::shared_ptr<QRObject>> objects;
};

class Scene3D: public BaseScene3D {
public:
    // todo passing value, std::move
    explicit Scene3D(const QRVector<std::shared_ptr<QRObject>> &obj, std::shared_ptr<QRCamera3D> cam)
    : objects(obj), activeCamera(cam),
    selectionManager(shared_ptr<BaseSelectionManager>(new SelectionManager(objects))) {}

    explicit Scene3D(std::shared_ptr<QRCamera3D> cam)
    :activeCamera(cam),
     selectionManager(shared_ptr<BaseSelectionManager>(new SelectionManager(objects))){}

    virtual bool addObject(std::shared_ptr<QRObject> obj) {objects.push_back(obj);}
    virtual void deleteObject(ObjectIterator obj) {
        if (selectionManager->isSelected(obj))
            selectionManager->switchSelection(obj);
        objects.pop(obj);
    }

    virtual void switchSelection(const ObjectIterator &it) {
        selectionManager->switchSelection(it);
    }
    virtual QRVector<shared_ptr<QRObject>> getSelection() {
        return selectionManager->getSelection();
    }
    virtual bool isEmptySelection() {return selectionManager->isEmptySelection();}

    virtual const QRVector<std::shared_ptr<QRObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<std::shared_ptr<QRObject>> &obj) { objects = obj;}
    virtual std::shared_ptr<QRCamera3D> getActiveCamera() {return activeCamera;}

private:
    QRVector<std::shared_ptr<QRObject>> objects;
    std::shared_ptr<QRCamera3D> activeCamera;
    std::shared_ptr<BaseSelectionManager> selectionManager;
};


#endif //BIG3DFLUFFY_SCENE_H

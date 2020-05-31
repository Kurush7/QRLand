//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENE_H
#define BIG3DFLUFFY_SCENE_H

#include "BaseComposite.h"
#include "../components/Camera3D.h"

// SCENE IS NOT COMPOSITE!

class SceneMemento;
class BaseScene3D {
public:
    BaseScene3D() {
        p = std::shared_ptr<BaseScene3D>(this, [](void *ptr){});}
    ~BaseScene3D() {p.reset();}

    // todo select-manager, selection
    virtual bool addObject(std::shared_ptr<BaseObject> obj) = 0;

    virtual std::shared_ptr<BaseScene3D> getPointer() {return p;}

    virtual const QRVector<std::shared_ptr<BaseObject>>& getObjects() = 0;
    virtual void setObjects(QRVector<std::shared_ptr<BaseObject>>&) = 0;
    virtual std::shared_ptr<BaseCamera3D> getActiveCamera() = 0;

    virtual std::unique_ptr<Memento> save();

    virtual bool operator==(const BaseScene3D &b) const = delete;
    virtual BaseScene3D& operator=(const BaseScene3D &p) = delete;

private:
    std::shared_ptr<BaseScene3D> p;
};

class SceneMemento: public Memento {
public:
    explicit SceneMemento(std::shared_ptr<BaseScene3D> scene);
    virtual void restore();
private:
    std::weak_ptr<BaseScene3D> scene;   // todo camera is keeped, but not restored, since there is no setCamera method
    std::weak_ptr<BaseCamera3D> camera;
    QRVector<std::shared_ptr<BaseObject>> objects;
};

class Scene3D: public BaseScene3D {
public:
    // todo passing value, std::move
    explicit Scene3D(const QRVector<std::shared_ptr<BaseObject>> &obj, std::shared_ptr<BaseCamera3D> cam)
    : objects(obj), activeCamera(cam) {}
    explicit Scene3D(std::shared_ptr<BaseCamera3D> cam): activeCamera(cam) {}

    virtual bool addObject(std::shared_ptr<BaseObject> obj) {objects.push_back(obj);}

    virtual const QRVector<std::shared_ptr<BaseObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<std::shared_ptr<BaseObject>> &obj) {objects = obj;}
    virtual std::shared_ptr<BaseCamera3D> getActiveCamera() {return activeCamera;}

private:
    QRVector<std::shared_ptr<BaseObject>> objects;
    std::shared_ptr<BaseCamera3D> activeCamera;
};


#endif //BIG3DFLUFFY_SCENE_H

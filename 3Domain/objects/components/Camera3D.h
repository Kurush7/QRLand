//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_CAMERA_H
#define BIG3DFLUFFY_CAMERA_H

#include "../BaseObject.h"
#include "Point3D.h"
#include "Projection.h"

class Camera3DMemento;
class BaseCamera3D: public Object3D {
public:
    BaseCamera3D(double w, double h, std::shared_ptr<ProjectionImp>pr): width(w), height(h), projector(pr){
        p = std::shared_ptr<BaseCamera3D>(this, [](void *ptr){});}
    ~BaseCamera3D() {p.reset();}

    virtual void acceptVisitor(std::shared_ptr<Visitor> visitor) {visitor->visitCamera3D(p);}

    virtual std::unique_ptr<Memento> save();
    virtual std::unique_ptr<BaseTransformer3D> getProjectionTransformer() = 0;

    virtual bool operator==(const BaseCamera3D &b) const {return getOrigin() == b.getOrigin() &&
                                                                getBind() == b.getBind() && width == b.width &&
                                                                height == b.height;}
    virtual BaseCamera3D& operator=(const BaseCamera3D &p) {}

// getters & setters
    double getWidth() const;
    void setWidth(double width);
    double getHeight() const;
    void setHeight(double height);

    virtual const std::shared_ptr<BaseQRPoint3D> getOrigin() const = 0;
    virtual void setOrigin(std::shared_ptr<BaseQRPoint3D>) = 0;

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D&) = 0;

    virtual const std::shared_ptr<ProjectionImp> getProjector() const {return projector;}
    virtual const void setProjector(std::shared_ptr<ProjectionImp> p) {projector = p;}

    std::weak_ptr<BaseCamera3D> getPointer() {return p;}

private:
    std::shared_ptr<BaseCamera3D> p;
protected:
    std::shared_ptr<ProjectionImp> projector;
    double width, height;
};

class Camera3DMemento: public Memento {
public:
    Camera3DMemento(std::shared_ptr<BaseCamera3D>);
    virtual void restore();

private:
    std::weak_ptr<BaseCamera3D> object;
    std::shared_ptr<BaseQRPoint3D> origin;
    double width, height;
    std::shared_ptr<ProjectionImp> projector;
};


class Camera3D: public BaseCamera3D {
public:
    Camera3D(double w, double h, std::shared_ptr<ProjectionImp>p, const Vector3D &origin,
             const Vector3D &viewUp = Vector3D(), const Vector3D &bind = Vector3D());

    virtual bool isVisible(){return false;}


    virtual std::unique_ptr<BaseTransformer3D> getProjectionTransformer();

    virtual const std::shared_ptr<BaseQRPoint3D> getOrigin() const {return origin;}
    virtual void setOrigin(std::shared_ptr<BaseQRPoint3D> p) {origin = p;}
    virtual const Vector3D& getBind() const {return origin->getBind();}
    virtual void setBind(const Vector3D &b) {origin->setBind(b);}

private:
    Vector3D viewUpVector;
    std::shared_ptr<BaseQRPoint3D> origin;
};


#endif //BIG3DFLUFFY_CAMERA_H

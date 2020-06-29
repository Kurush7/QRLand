//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_VISITORIMPL_H
#define BIG3DFLUFFY_VISITORIMPL_H

#include "QRVisitorInterface.h"

#include "../components/interfaces.h"
#include "../composites/interfaces.h"
#include "objects/QRObject.h"
#include "../../QRConstants.h"

class TransformVisitor: public QRVisitor {
public:
    TransformVisitor(sptr<QRTransformer3D> t): transformer(t) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);
    virtual void visitFrame3D(sptr<QRFrame3D> frame);

protected:
    sptr<QRTransformer3D> transformer;
};

class MoveTransformVisitor: public TransformVisitor {
public:
    MoveTransformVisitor(sptr<QRTransformer3D> t): TransformVisitor(t) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitFrame3D(sptr<QRFrame3D> frame);
protected:
    Vector3D frameBind;
};

class BindSetterVisitor: public QRVisitor {
public:
    BindSetterVisitor(const Vector3D &bind): bind(bind) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

protected:
    Vector3D bind;
};

class ScaleCameraVisitor: public TransformVisitor {
public:
    ScaleCameraVisitor(sptr<QRTransformer3D> t): TransformVisitor(t) {}
    virtual void visitCamera3D(sptr<QRCamera3D> camera);
};

class SelectionVisitor: public QRVisitor {
public:
    SelectionVisitor(double x, double y, sptr<QRTransformer3D> t): x(x), y(y), transformer(t) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

    bool is_selected;
protected:
    sptr<QRTransformer3D> transformer;
    double x, y;
};

class SetColorVisitor: public QRVisitor {
public:
    SetColorVisitor(ColorKeeper keeper): keeper(keeper) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

    bool is_selected;
protected:
    ColorKeeper keeper;
};

#endif //BIG3DFLUFFY_QRVISITOR_H

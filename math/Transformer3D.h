//
// Created by kurush on 29.05.2020.
//

#ifndef KG_TRANSFORMER3D_H
#define KG_TRANSFORMER3D_H

#include <memory>
#include <cmath>

#include "Matrix3D.h"


// todo creators return shared, not unique????? NO
// todo get rid of interface, create just transformers


class QRTransformer3D {
public:
    virtual Matrix3D transformRight(const Matrix3D &m) = 0;
    virtual Matrix3D transformLeft(const Matrix3D &m) = 0;
    virtual Matrix3D transform(const Matrix3D &m) {return transformLeft(m);}

    virtual Vector3D transform(const Vector3D &v) = 0;

    virtual Matrix3D getMatrix() = 0;

    virtual void accumulateRight(const Matrix3D &m) = 0;
    virtual void accumulateLeft(const Matrix3D &m) = 0;
    virtual void accumulate(const Matrix3D &m) {accumulateLeft(m);};

    virtual QRTransformer3D& operator=(const Matrix3D&) = 0;
};

class Transformer3D: public QRTransformer3D {
public:
    explicit Transformer3D(const Matrix3D &m = makeID()): matrix(m) {}
    virtual Matrix3D transformRight(const Matrix3D &m) {return m * matrix;}
    virtual Matrix3D transformLeft(const Matrix3D &m) {return matrix * m;}

    virtual Vector3D transform(const Vector3D &v) {return matrix * v;}

    virtual void accumulateRight(const Matrix3D &m) {matrix = matrix * m;};
    virtual void accumulateLeft(const Matrix3D &m) {matrix = m * matrix;};

    virtual QRTransformer3D& operator=(const Matrix3D &m) {matrix = m; return *this;}

    virtual Matrix3D getMatrix() {return matrix;}

protected:
    Matrix3D matrix;
};

// todo as creator
class AxisChangeTransformer: public Transformer3D {
public:
    AxisChangeTransformer(const Vector3D &oX,
                          const Vector3D &oY, const Vector3D &oZ,
                          const Vector3D &origin = ZeroVector);
};



class BaseTransformer3DCreator {
public:
    virtual uptr<QRTransformer3D> create() = 0;
};
class MoveTransformer3DCreator: public BaseTransformer3DCreator {
public:
    MoveTransformer3DCreator(float dx, float dy, float dz);
    MoveTransformer3DCreator(const Vector3D &v): MoveTransformer3DCreator(v[0],v[1],v[2]) {}
    virtual uptr<QRTransformer3D> create();
private:
    Matrix3D matrix;
};
class ScaleTransformer3DCreator: public BaseTransformer3DCreator {
public:
    ScaleTransformer3DCreator(float kx, float ky, float kz);
    ScaleTransformer3DCreator(const Vector3D &v): ScaleTransformer3DCreator(v[0],v[1],v[2]) {}
    virtual uptr<QRTransformer3D> create();
private:
    Matrix3D matrix;
};
class RotateTransformer3DCreator: public BaseTransformer3DCreator {
public:
    RotateTransformer3DCreator(float dx, float dy, float dz);
    RotateTransformer3DCreator(const Vector3D &v): RotateTransformer3DCreator(v[0],v[1],v[2]) {}
    virtual uptr<QRTransformer3D> create();
private:
    enum axis {ox, oy, oz};
    Matrix3D createOneRotateMatrix(float rad, axis ax);

    Matrix3D matrix;
};
class ProjectionTransformer3DCreator: public BaseTransformer3DCreator {
public:
    ProjectionTransformer3DCreator(float z);
    virtual uptr<QRTransformer3D> create();
private:
    Matrix3D matrix;
};
#endif //KG_TRANSFORMER3D_H

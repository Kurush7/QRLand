//
// Created by kurush on 29.05.2020.
//

#ifndef KG_TRANSFORMER3D_H
#define KG_TRANSFORMER3D_H

#include <memory>
#include <cmath>

#include "Matrix3D.h"


class BaseTransformer3D {
public:
    virtual Matrix3D transformRight(const Matrix3D &m) = 0;
    virtual Matrix3D transformLeft(const Matrix3D &m) = 0;

    virtual Vector3D transform(const Vector3D &v) = 0;

    virtual void accumulateRight(const Matrix3D &m) = 0;
    virtual void accumulateLeft(const Matrix3D &m) = 0;
};

class Transformer3D: public BaseTransformer3D {
public:
    Transformer3D(const Matrix3D &m): matrix(m) {}
    virtual Matrix3D transformRight(const Matrix3D &m) {return m * matrix;}
    virtual Matrix3D transformLeft(const Matrix3D &m) {return matrix * m;}

    virtual Vector3D transform(const Vector3D &v) {return matrix * v;}

    virtual void accumulateRight(const Matrix3D &m) {matrix *= m;};
    virtual void accumulateLeft(const Matrix3D &m) {matrix = m * matrix;};

private:
    Matrix3D matrix;
};


class BaseTransformer3DCreator {
public:
    virtual std::unique_ptr<BaseTransformer3D> create() = 0;
};

class MoveTransformer3DCreator: public BaseTransformer3DCreator {
public:
    MoveTransformer3DCreator(double dx, double dy, double dz);
    virtual std::unique_ptr<BaseTransformer3D> create();
private:
    Matrix3D matrix;
};

class ScaleTransformer3DCreator: public BaseTransformer3DCreator {
public:
    ScaleTransformer3DCreator(double kx, double ky, double kz);
    virtual std::unique_ptr<BaseTransformer3D> create();
private:
    Matrix3D matrix;
};

class RotateTransformer3DCreator: public BaseTransformer3DCreator {
public:
    RotateTransformer3DCreator(double dx, double dy, double dz);
    virtual std::unique_ptr<BaseTransformer3D> create();
private:
    enum axis {ox, oy, oz};
    Matrix3D createOneRotateMatrix(double rad, axis ax);

    Matrix3D matrix;
};

// todo projection transformer
/*class ProjectionTransformer3DCreator: public BaseTransformer3DCreator {
public:
    ProjectionTransformer3DCreator(double kx, double ky, double kz);
    virtual std::unique_ptr<BaseTransformer3D> create();
private:
    Matrix3D matrix;
};*/

#endif //KG_TRANSFORMER3D_H

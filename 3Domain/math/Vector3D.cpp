//
// Created by kurush on 29.05.2020.
//

#include "Vector3D.h"

Vector3D::Vector3D() {}
Vector3D::Vector3D(double x, double y, double z) {
    arr[0] = x, arr[1] = y, arr[2] = z;
}
Vector3D::Vector3D(double x, double y, double z, double p) {
    arr[0] = x, arr[1] = y, arr[2] = z, arr[3] = p;
}
Vector3D::Vector3D(const double _arr[4]) {
    arr[0] = _arr[0], arr[1] = _arr[1];
    arr[2] = _arr[2], arr[3] = _arr[3];
}

Vector3D::Vector3D(const std::initializer_list<double> &lst) {
    if (lst.size() != 3) {
        time_t t = time(nullptr);
        throw QRMathWrongDimension(__FILE__, __LINE__, asctime(localtime(&t)),
                                  "Bad vector init-list!", lst.size(), 3);
    }
    char i = 0;
    for (auto x: lst) {
        arr[i++] = x;
    }
}

double& Vector3D::operator[](int ind) {
    if (ind < 0 || ind > 3) {
        time_t t = time(nullptr);
        throw QRMathWrongDimension(__FILE__, __LINE__, asctime(localtime(&t)),
                                   "Bad vector index!", ind, 3);
    }
    return arr[ind];
}

const double& Vector3D::operator[](int ind) const {
    if (ind < 0 || ind > 3) {
        time_t t = time(nullptr);
        throw QRMathWrongDimension(__FILE__, __LINE__, asctime(localtime(&t)),
                                   "Bad vector index!", ind, 3);
    }
    return arr[ind];
}

Vector3D& Vector3D::operator +=(const Vector3D &v) {
    arr[0] += v.arr[0], arr[1] += v.arr[1];
    arr[2] += v.arr[2], arr[3] += v.arr[3];
    return *this;
}
Vector3D& Vector3D::operator -=(const Vector3D &v) {
    arr[0] -= v.arr[0], arr[1] -= v.arr[1];
    arr[2] -= v.arr[2], arr[3] -= v.arr[3];
    return *this;
}

Vector3D& Vector3D::operator /=(double x) {
    arr[0] /= x, arr[1] /= x;
    arr[2] /= x, arr[3] /= x;
    return *this;
}

bool operator ==(const Vector3D &a, const Vector3D &b) {
    return a.arr[0] == b.arr[0] && a.arr[1] == b.arr[1] &&
           a.arr[2] == b.arr[2] && a.arr[3] == b.arr[3];
}
bool operator !=(const Vector3D &a, const Vector3D &b) {
    return a.arr[0] != b.arr[0] || a.arr[1] != b.arr[1] ||
           a.arr[2] != b.arr[2] || a.arr[3] != b.arr[3];
}
Vector3D operator +(const Vector3D &a, const Vector3D &b) {
    Vector3D c = a;
    c += b;
    return c;
}
Vector3D operator -(const Vector3D &a, const Vector3D &b) {
    Vector3D c = a;
    c -= b;
    return c;
}

Vector3D operator *(const Vector3D &a0, const Vector3D &b0) {
    Vector3D c, a = a0, b = b0;
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
    return c;
}

Vector3D operator *(const Vector3D &a, double x) {
    Vector3D b = a;
    b[0] *= x;
    b[1] *= x;
    b[2] *= x;
    return b;
}
Vector3D operator *(double x, const Vector3D &a) {
    Vector3D b = a;
    b[0] *= x;
    b[1] *= x;
    b[2] *= x;
    return b;
}

Vector3D operator /(const Vector3D &a, double x) {
    Vector3D b = a;
    b[0] /= x;
    b[1] /= x;
    b[2] /= x;
    return b;
}

Vector3D norm(const Vector3D &a) {
    Vector3D b = a;
    if (fabs(b[3] - 1) < QREPS)
        return b;
    if (fabs(b[3]) < QREPS)
        b[3] = 1;
    b[0] = b[0]/ b[3];
    b[1] = b[1]/ b[3];
    b[2] = b[2]/ b[3];
    b[3] = 1;
    return b;
}

Vector3D lenNorm(const Vector3D &a) {
    Vector3D b = a;
    double x = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2] + a[3]*a[3]);
    if (fabs(x) > QREPS) {
        b[0] /= x;
        b[1] /= x;
        b[2] /= x;
        b[3] /= x;
        // todo needed b[3] /= x: check its usages
    }
    return b;
}

double vectorLen(const Vector3D &v) {
    // todo v[3] considered. all is good, yet accuracy needed....(((
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
}
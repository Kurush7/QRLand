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
Vector3D::Vector3D(const float _arr[4]) {
    arr[0] = _arr[0], arr[1] = _arr[1];
    arr[2] = _arr[2], arr[3] = _arr[3];
}

Vector3D::Vector3D(const std::initializer_list<float> &lst) {
    char i = 0;
    for (auto x: lst) {
        arr[i++] = x;
    }
}

float& Vector3D::operator[](int ind) {
    return arr[ind];
}

const float& Vector3D::operator[](int ind) const {
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

Vector3D& Vector3D::operator /=(float x) {
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
    return Vector3D({a.arr[0]+b.arr[0], a.arr[1]+b.arr[1], a.arr[2]+b.arr[2], a.arr[3]+b.arr[3]});
}
Vector3D operator -(const Vector3D &a, const Vector3D &b) {
    return Vector3D({a.arr[0]-b.arr[0], a.arr[1]-b.arr[1], a.arr[2]-b.arr[2], a.arr[3]-b.arr[3]});
}

Vector3D operator *(const Vector3D &a0, const Vector3D &b0) {
    Vector3D c, a = a0, b = b0;
    return Vector3D({a[1]*b[2] - a[2]*b[1],
                     a[2]*b[0] - a[0]*b[2],
                     a[0]*b[1] - a[1]*b[0],
                     0});
}

Vector3D operator *(const Vector3D &a, float x) {
    return Vector3D({a.arr[0]*x, a.arr[1]*x, a.arr[2]*x, a.arr[3]*x});
}
Vector3D operator *(float x, const Vector3D &a) {
    return Vector3D({a.arr[0]*x, a.arr[1]*x, a.arr[2]*x, a.arr[3]*x});
}

Vector3D operator /(const Vector3D &a, float x) {
    return Vector3D({a.arr[0]/x, a.arr[1]/x, a.arr[2]/x, a.arr[3]/x});
}

Vector3D norm(const Vector3D &a) {
    float x = a[3];
    if (fabs(x) < QREPS) return a;
    return a / x;
}
void Vector3D::normSelf() {
    if (fabs(arr[3]) < QREPS) return;
    arr[0] /= arr[3];
    arr[1] /= arr[3];
    arr[2] /= arr[3];
    arr[3] = 1;
}

Vector3D lenNorm(const Vector3D &a) {
    float x = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2] + a[3]*a[3]);
    if (fabs(x) < QREPS) return a;
    return a / x;
}
void Vector3D::lenNormSelf() {
    float x = sqrt(arr[0]*arr[0]+arr[1]*arr[1]+arr[2]*arr[2] + arr[3]*arr[3]);
    arr[0] /= x;
    arr[1] /= x;
    arr[2] /= x;
    arr[3] /= x;
}

float vectorLen(const Vector3D &v) {
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
}

float vectorLen3(const Vector3D &v) {
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

float scalar(const Vector3D &a, const Vector3D &b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

float scalar3(const Vector3D &a, const Vector3D &b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}


bool isRightRotate(const Vector3D &a, const Vector3D &b, const Vector3D &c) {
    auto v1 = b-a, v2 =  c-a;
    return sign(v1[0] * v2[1] - v1[1] * v2[0]) < 0;
}

float cos(const Vector3D &a, const Vector3D &b) {
    return scalar(a,b) / vectorLen(a) / vectorLen(b);
}

float cos3(const Vector3D &a, const Vector3D &b) {
    return scalar3(a,b) / vectorLen3(a) / vectorLen3(b);
}

std::ostream& operator<<(std::ostream &os, const Vector3D &v) {
    os << "<Vector3D: " << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << ">";
    return os;
}
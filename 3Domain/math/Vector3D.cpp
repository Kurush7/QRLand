//
// Created by kurush on 29.05.2020.
//

#include "Vector3D.h"

Vector3D::Vector3D() {}
Vector3D::Vector3D(double x, double y, double z) {
    arr[0] = x, arr[1] = y, arr[2] = z;
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
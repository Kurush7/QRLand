//
// Created by kurush on 29.05.2020.
//

#include "Matrix3D.h"


Matrix3D makeID() {
    return Matrix3D();
}
Matrix3D makeNull() {
    Matrix3D ans;
    for (int i = 0; i <= 3; ++i)
        ans.matrix[i][i] = 0;
    return ans;
}

Matrix3D::Matrix3D() {
    for (int i = 0; i <= 3; ++i) {
        matrix[i][i] = 1;
    }
}
Matrix3D::Matrix3D(const std::initializer_list<std::initializer_list<double>> &lst) {
    if (lst.size() != 4)
        throw QRMathWrongDimension(__FILE__, __LINE__, __TIME__,
                                   "Bad matrix init-list (rows)!", lst.size(), 4);
    int i = 0, j = 0;
    for (auto line: lst) {
        if (line.size() != 4)
            throw QRMathWrongDimension(__FILE__, __LINE__, __TIME__,
                                       "Bad matrix init-list (columns)!", lst.size(), 4);
        for (auto x: line)
            matrix[i][j++] = x;
        j = 0;
        i++;
    }
}

Vector3D& Matrix3D::operator[](int ind) {
    if (ind < 0 || ind > 3)
        throw QRMathWrongDimension(__FILE__, __LINE__, __TIME__,
                                   "Bad matrix row index!", ind, 3);
    return matrix[ind];
}

const Vector3D& Matrix3D::operator[](int ind) const{
    if (ind < 0 || ind > 3) {
        time_t t = time(nullptr);
        throw QRMathWrongDimension(__FILE__, __LINE__, asctime(localtime(&t)),
                                   "Bad matrix row index!", ind, 3);
    }
    return matrix[ind];
}

Matrix3D& Matrix3D::operator +=(const Matrix3D &m) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            matrix[i][j] += m[i][j];
    return *this;
}
Matrix3D& Matrix3D::operator *=(const Matrix3D &a) {
    auto m = (*this * a);
    for (int i = 0; i < 4; ++i)
        matrix[i] = m[i];
    return *this;
}


bool operator ==(const Matrix3D &a, const Matrix3D &b) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (a[i][j] != b[i][j])
                return false;
    return true;
}
bool operator !=(const Matrix3D &a, const Matrix3D &b) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (a[i][j] != b[i][j])
                return false;
    return true;
}

Matrix3D operator +(const Matrix3D &a, const Matrix3D &b) {
    Matrix3D c = a;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            c[i][j] += b[i][j];
    return c;
}
Matrix3D operator *(const Matrix3D &a, const Matrix3D &b) {
    Matrix3D c = makeNull();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                c[i][j] += a[i][k] * b[k][j];
    return c;
}
Vector3D operator *(const Matrix3D &m, const Vector3D &v0) {
    Vector3D ans, v = v0;
    v[3] = 1;   // todo fuck

    for (int i = 0; i < 4; ++i) {
        ans[i] = 0;
            for (int k = 0; k < 4; ++k)
                ans[i] += m[i][k] * v[k];
    }
    //ans[3] = 0;     // todo fuck?
    return ans;
}

std::ostream& operator<<(std::ostream &os, const Matrix3D &m) {
    os << "<Matrix3D>:\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j)
            os << m[i][j] << ' ';
        os <<'\n';
    }
}

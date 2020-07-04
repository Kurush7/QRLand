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
    int i = 0, j = 0;
    for (auto line: lst) {
        for (auto x: line) matrix[i][j++] = x;
        j = 0;
        i++;
    }
}

Vector3D& Matrix3D::operator[](int ind) {
    return matrix[ind];
}

const Vector3D& Matrix3D::operator[](int ind) const{
    return matrix[ind];
}

Matrix3D& Matrix3D::operator +=(const Matrix3D &m) {
    matrix[0][0] += m[0][0];
    matrix[0][1] += m[0][1];
    matrix[0][2] += m[0][2];
    matrix[0][3] += m[0][3];
    matrix[1][0] += m[1][0];
    matrix[1][1] += m[1][1];
    matrix[1][2] += m[1][2];
    matrix[1][3] += m[1][3];
    matrix[2][0] += m[2][0];
    matrix[2][1] += m[2][1];
    matrix[2][2] += m[2][2];
    matrix[2][3] += m[2][3];
    matrix[3][0] += m[3][0];
    matrix[3][1] += m[3][1];
    matrix[3][2] += m[3][2];
    matrix[3][3] += m[3][3];
    return *this;
}
Matrix3D& Matrix3D::operator *=(const Matrix3D &a) {
    auto c = ((*this)*a).matrix;
    for (int i = 0; i < 4; ++i)
        matrix[i] = c[i];
    return (*this);
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

Matrix3D operator +(const Matrix3D &a, const Matrix3D &m) {
    Matrix3D c = a;
    c[0][0] += m[0][0];
    c[0][1] += m[0][1];
    c[0][2] += m[0][2];
    c[0][3] += m[0][3];
    c[1][0] += m[1][0];
    c[1][1] += m[1][1];
    c[1][2] += m[1][2];
    c[1][3] += m[1][3];
    c[2][0] += m[2][0];
    c[2][1] += m[2][1];
    c[2][2] += m[2][2];
    c[2][3] += m[2][3];
    c[3][0] += m[3][0];
    c[3][1] += m[3][1];
    c[3][2] += m[3][2];
    c[3][3] += m[3][3];
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
Vector3D operator *(const Matrix3D &m, const Vector3D &v) {
    return Vector3D({
        m[0][0]*v[0]+m[0][1]*v[1] + m[0][2]*v[2] + m[0][3],
    m[1][0]*v[0]+m[1][1]*v[1] + m[1][2]*v[2] + m[1][3],
    m[2][0]*v[0]+m[2][1]*v[1] + m[2][2]*v[2] + m[2][3],
    m[3][0]*v[0]+m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]});
}

std::ostream& operator<<(std::ostream &os, const Matrix3D &m) {
    os << "<Matrix3D>:\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j)
            os << m[i][j] << ' ';
        os <<'\n';
    }
}

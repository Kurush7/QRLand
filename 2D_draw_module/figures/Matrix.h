//
// Created by kurush on 20.02.2020.
//

#ifndef KG_MATRIX_H
#define KG_MATRIX_H

#include <vector>

using namespace std;

template <class T>
class Matrix {
public:
    Matrix(int n, int m): n(n), m(m), matrix(n, vector<T>(m, 0)) {};
    Matrix(int n, int m, T val): n(n), m(m), matrix(n, vector<T>(m, val)) {};
    Matrix(int n, int m, vector<T> val): n(n), m(m), matrix(n, vector<T>(m))
    {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                matrix[i][j] = val[i*m + j];
    }

    vector<T>& operator [](int i)
    {
        return matrix[i];
    }
    int n, m;
private:
    vector<vector<T>> matrix;
};

template <class T>
Matrix<T> product(Matrix<T> A, Matrix<T> B)
{
    if (A.m != B.n)
        return Matrix<T>(0,0);

    Matrix<T> C(A.n, B.m, 0);
    for (int i = 0; i < A.n; ++i)
        for (int j = 0; j < B.m; ++j)
            for (int k = 0; k < A.m; ++k)
                C[i][j] += A[i][k] * B[k][j];

    return C;
}

template <class T>
Matrix<T> IDMatrix(int n)
{
    Matrix<T> id (n, n, 0);
    for (int i = 0; i < n; ++i)
        id[i][i] = 1;
    return id;
}


#endif //KG_MATRIX_H

//
// Created by kurush on 23.05.2020.
//

#ifndef KG_FUNCTION3D_H
#define KG_FUNCTION3D_H

#include <string>
#include <functional>
#include <math.h>
#include "Primitives.h"
#include <map>

struct Data {
    double xMin, xMax, yMin, yMax, zMin, zMax;
    int xCnt, yCnt, zCnt;
    std::array<std::array<double, 3>, 3> matrix;
};
struct Point3D {
    Point3D(double x, double y, double z): x(x), y(y), z(z) {}
    Point3D(): x(0), y(0), z(0) {}
    double x, y, z;
};

template <typename _Key, typename _Val>
class default_map: public std::map<_Key, _Val> {
public:
    default_map(): std::map<_Key, _Val>() {}
    default_map(_Val def): std::map<_Key, _Val>(), _default(def) {}
    _Val& operator [](_Key key) {
        auto it = this->find(key);
        if (it == this->end()) {
            std::map<_Key, _Val>::operator[](key) = _default;
            it = this->find(key);
        }
        return it->second;
    }
private:
    _Val _default;
};

class smart_vector: public std::vector<double> {
public:
    ~smart_vector() {
        delete arr;
    }
    smart_vector(double left, double right, int len, double def): left(left), right(right), len(len) {
        step = (right - left) / len;
        arr = new double[len];
        for (int i = 0; i < len; ++i)
            arr[i] = def;
    }
    double& operator[](double x) {
        int i = (x - left) / step;
        i = std::max(i, 0);
        i = std::min(i, len-1);
        return arr[i];
    }

private:
    int len;
    double left, right, step;
    double *arr;
};

class Function3D {
public:
    Function3D(std::string name, std::function<double(double, double, double)> func)
    : name(name), func(func) {}
    std::string getName() {return name;}
    double operator ()(double x, double y, double z) const {return func(x,y, z);}
private:
    std::string name;
    std::function<double(double, double, double)> func;
};

std::vector<Line> buildFunc(const Function3D&, const Data&, int k = 100);

double sin_sqrt(double x, double y, double z);
double example(double x, double y, double z);
double plane(double x, double y, double z);
double f1(double x, double y, double z);
double f2(double x, double y, double z);
double f3(double x, double y, double z);
double f4(double x, double y, double z);
double f5(double x, double y, double z);


#endif //KG_FUNCTION3D_H

//
// Created by kurush on 23.05.2020.
//

#include "Function3D.h"
#include <iostream>
#include <queue>

using namespace std;

double sin_sqrt(double x, double y, double z) {
    return sin(sqrt(x*x + y*y)) - z;
}
double example(double x, double y, double z) {
    double a = (x-M_PI)*(x-M_PI)+(y-M_PI)*(y-M_PI);
    return 1/5*sin(x)*cos(y) - 1.5*cos(7*a/4)*exp(-a) - z;
}
double plane(double x, double y, double z) {
    return x + z - y;
}
double f1(double x, double y, double z) {
    return x*x/4 - y*y/9 - 2*z;
}
double f2(double x, double y, double z) {
    return -z + exp(-(x*x+y*y)/8)*(sin(x*x)+cos(y*y));
}
double f3(double x, double y, double z) {
    return -z + 5/(1+x*x)+5/(1+y*y);
}
double f4(double x, double y, double z) {
    return -z + x*x+y*y;
}
double f5(double x, double y, double z) {
    return -z + sin(x)*cos(y) / (x+1);
}




Point3D prod(Point3D a, array<array<double, 3>, 3> matr) {
    Point3D ans;
    ans.x = a.x*matr[0][0] + a.y*matr[0][1] + a.z*matr[0][2];
    ans.y = a.x*matr[1][0] + a.y*matr[1][1] + a.z*matr[1][2];
    ans.z = a.x*matr[2][0] + a.y*matr[2][1] + a.z*matr[2][2];
    return ans;
}
Point3D norm(Point3D a) {
    Point3D b = a;
    double len = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    b.x /= len;
    b.y /= len;
    b.z /= len;
    return b;
}
Point3D operator *(Point3D a, double x) {
    Point3D b = a;
    b.x *= x;
    b.y *= x;
    b.z *= x;
    return b;
}
Point3D operator +(Point3D a, Point3D b) {
    Point3D c = a;
    c.x += b.x;
    c.y += b.y;
    c.z += b.z;
    return c;
}
Point3D transform(double x, double y, double z, array<array<double, 3>, 3> matrix)
{
    Point3D p;
    for (int k = 0; k < 3; ++k) {
        p.x += matrix[0][k] * x;
        p.y += matrix[1][k] * y;
        p.z += matrix[2][k] * z;
    }
    return p;
}

/*pair<double, bool> find_root(const Function3D &f, Point3D pt, Point3D dir, double l, double r, double eps=1e-2, int max_iter = 1000) {
    Point3D p1 = pt + dir*l, p2 = pt + dir*r, p3;
    if (sign(f(p1.x, p1.y, p1.z)) * sign(f(p2.x, p2.y, p2.z)) == 1)
        return make_pair(0, false);
    int iter_cnt = 0;
    double m, rt;
    while (1) {
        if (iter_cnt > max_iter)
            return make_pair(0, false);
        m = (l + r) / 2;
        p3 = pt + dir*m;
        if (sign(f(p1.x, p1.y, p1.z)) * sign(f(p3.x, p3.y, p3.z)) == 1)
            l = m, p1 = pt + dir*l;
        else
            r = m, p2 = pt + dir*r;
        rt = (l + r) / 2;
        if (fabs(r - l) < fabs(rt) * eps + eps)
            return make_pair(rt, true);
        iter_cnt++;
    }
}
pair<double, double> localize_root(const Function3D &f, Point3D pt, Point3D dir, double l, double r, double eps=1e-2, int max_iter = 1000) {
    double factor = 0.1;
    Point3D p1 = pt + dir*l, p2 = pt + dir*r;
    double f1 = f(p1.x, p1.y, p1.z),
           f2 = f(p2.x, p2.y, p2.z);
    for (int i = 0; i < max_iter; ++i) {
        if (f1 * f2 < -eps)
            return make_pair(l, r);
        if (fabs(f1) -  fabs(f2) < -eps) {
            l += factor * (r-l);
            p1 = pt + dir*l;
            f1 = f(p1.x, p1.y, p1.z);
        }
        else {
            r += factor * (l - r);
            p2 = pt + dir*r;
            f2 = f(p2.x, p2.y, p2.z);
        }
    }
    return make_pair(0,0);
}
vector<double> find_roots(const Function3D &f, Point3D pt, Point3D dir, double l, double r, double eps=1e-2) {
    queue<pair<double, double>> q;
    q.push(make_pair(l, r));
    vector<double> roots;
    double a, b;
    pair<double, double> zone;
    pair<double, bool> root;
    while (!q.empty()) {
        a = q.front().first, b = q.front().second;
        q.pop();
        zone = localize_root(f, pt, dir, a, b, eps);
        if (zone.first != zone.second) {
            root = find_root(f, pt, dir, zone.first, zone.second, eps);
            if (root.second) {
                roots.push_back(root.first);
                q.push(make_pair(zone.first, root.first - 10*eps));
                q.push(make_pair(root.first + 10*eps, zone.second));
            }
        }
    }
    return roots;
}*/
double findVal(Function3D f, double zmin, double zmax, Point3D pt, Point3D dir, int cnt, double eps=1e-2) {
    double step = (zmax - zmin) / cnt, dz = zmin;
    Point3D cur;
    for (int i = 0; i < cnt; ++i) {
        cur = pt + dir * dz;
        dz += step;
        if (fabs(f(cur.x, cur.y, cur.z)) < eps)
            return dz;
    }
    return zmin - 1;
}
vector<vector<Point3D>> get_grid(const Function3D &f, const Data &data) {
    vector<vector<Point3D>> ans;
    vector<Point3D> line;

    Point3D vx(1,0,0), vy(0,1,0), vz(0,0,1);
    vx = prod(vx, data.matrix);
    vy = prod(vy, data.matrix);
    vz = prod(vz, data.matrix);

    double dx = (data.xMax - data.xMin) / data.xCnt;
    vx = norm(vx) * dx;
    double dy = (data.yMax - data.yMin) / data.yCnt;
    vy = norm(vy) * dy;
    vz = norm(vz);
    vector<double> z_vec;
    double z;

    Point3D start(data.xMin, data.yMin, 0);
    start = prod(start, data.matrix);
    Point3D tmp, pt;

    for (int i = 0; i < data.yCnt+1; ++i) {
        line.clear();
        tmp = start;
        start = start + vy;
        for (int j = 0; j < data.xCnt+1; ++j) {
            z = findVal(f, data.zMin, data.zMax, tmp, vz, data.zCnt);
            if (z >= data.zMin && z <= data.zMax) {
                pt = Point3D(data.xMin + j*dx, data.yMin + i*dy, z);
                line.push_back(pt);
            }
            tmp = tmp + vx;
        }
        ans.push_back(line);
    }
    return ans;
}

Point intersect(const Point &a, const Point &b, smart_vector &horizon)
{
    Point i;
    double h1 = horizon[a.x], h2 = horizon[b.x];
    double dx = b.x - a.x, dy = b.y - a.y;
    double dh = h2 - h1;

    if (fabs(dx) < EPS) {
        i.x = b.x;
        i.y = h2;
    }
    else if (fabs(dy - dh) < EPS) {
        i.x = (b.x + a.x) / 2;
        i.y = horizon[i.x];
    }
    else {
        double m = dy / dx;
        i.x = a.x - dx * (a.y - h1) / (dy - dh);
        i.y = (i.x - a.x) * m + a.y;
    }
    return i;
}
void horizon(vector<Line> &ans, smart_vector &up, smart_vector &down,
             Point a, Point b, double eps = 0.01)
{
    if (b.x < a.x)
        swap(a, b);
    ans.push_back(Line(a, b));

    if (fabs(b.x - a.x) < EPS) {
        up[b.x] = max(up[b.x], b.y);
        down[b.x] = min(down[b.x], b.y);
    }
    else {
        auto p = a;
        double m = (b.y - a.y) / (b.x - a.x);
        double y = a.y;

        double step = (b.x - a.x) * eps;
        m *= step;
        for (double x = a.x; x <= b.x; x+=step) {
            up[x] = max(up[x], y);
            down[x] = min(down[x], y);
            y += m;
        }
    }
}
int visible(Point a, smart_vector &up, smart_vector &down)
{
    return (a.y - up[a.x]) > -EPS? 1 : (a.y - down[a.x]) < EPS? -1 : 0;
}

vector<Line> buildFunc(const Function3D &f, const Data &data, int k)
{
    vector<Line> ans;
    auto grid = get_grid(f, data);
    int n = grid.size(), m;

    smart_vector up(data.xMin, data.xMax, (data.xCnt+1)*k, data.zMin);
    smart_vector down(data.xMin, data.xMax, (data.xCnt+1)*k, data.zMax);

    Point prev, cur, inter, l, r;
    int prev_vis, cur_vis;
    bool firstBorder = true;

    for (int i = 0; i < n; ++i) {
        m = grid[i].size();
        if (!m) continue;   // в сечении нет точек пересечения с поверхностью

        prev.x = grid[i][0].x;
        prev.y = grid[i][0].z;
        prev_vis = visible(prev, up, down);

        if (!firstBorder) {     // обработка левого бокового ребра
            horizon(ans, up, down, l, prev);
        }
        l = prev;

        for (int j = 1; j < m; ++j) {
            cur.x = grid[i][j].x;
            cur.y = grid[i][j].z;
            cur_vis = visible(cur, up, down);

            if (cur_vis == prev_vis) {
                if (prev_vis)
                    horizon(ans, up, down, prev, cur);
            }
            else if (!cur_vis) {
                inter = intersect(prev, cur, prev_vis == 1 ? up : down);
                horizon(ans, up, down, prev, inter);
            }
            else if (cur_vis == 1) {
                inter = intersect(prev, cur, up);
                horizon(ans, up, down, inter, cur);

                if (prev_vis) {
                    inter = intersect(prev, cur, down);
                    horizon(ans, up, down, prev, inter);
                }
            }
            else {
                inter = intersect(prev, cur, down);
                horizon(ans, up, down, inter, cur);

                if (prev_vis) {
                    inter = intersect(prev, cur, up);
                    horizon(ans, up, down, prev, inter);
                }
            }

            prev_vis = cur_vis;
            prev = cur;
        }

        if (!firstBorder) { // обработка правого бокового ребра
            horizon(ans, up, down, r, prev);
            r = prev;
        } else {
            r = prev;
            firstBorder = false; }
    }
    return ans;
}
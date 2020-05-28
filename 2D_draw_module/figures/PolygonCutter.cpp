//
// Created by kurush on 14.05.2020.
//

#include "PolygonCutter.h"
#include <iostream>
#include <stack>

using namespace std;

inline char checkVisibility(const Point &a, const Point &p1, const Point &p2) {
    return -sign((a.x-p1.x) * (p2.y-p1.y) -
                    (a.y-p1.y) * (p2.x-p1.x));
}

inline bool checkIntersection(const Point &a, const Point &b, const Point &w1, const Point &w2) {
    char t1 = checkVisibility(a, w1, w2);
    char t2 = checkVisibility(b, w1, w2);
    return t1 * t2 < 0;
}

Point intersectionPoint(const Point &p1, const Point &p2, const Point &w1, const Point &w2) {
    double pa = p1.y - p2.y, wa = w1.y - w2.y;
    double pb = p2.x - p1.x, wb = w2.x - w1.x;
    double pc = - pa * p1.x - pb * p1.y;
    double wc = - wa * w1.x - wb * w1.y;
    double zn = pa*wb -  pb*wa;
    Point res;
    res.x = -(pc*wb - pb*wc) / zn;
    res.y = -(pa*wc - pc*wa) / zn;
    return res;
}

bool cutPolygon(const Polygon &cutter, const Polygon &poly, Polygon &res) {
    vector<Point> P = poly.points, W = cutter.points, Q;
    W.push_back(W[0]);
    int Np = P.size(), Nw = W.size();
    Point S, interP;
    bool interFlag;

    for (int i = 0; i < Nw - 1; ++i) {
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(S, P[j], W[i], W[i + 1]);
                if (interFlag) {
                    interP = intersectionPoint(S, P[j], W[i], W[i + 1]);
                    Q.push_back(interP);
                }
            }

            S = P[j];
            char visible = checkVisibility(S, W[i], W[i + 1]);
            if (visible >= 0)
                Q.push_back(S);
        }

        if (!Q.empty()) {
            interFlag = checkIntersection(S, P[0], W[i], W[i + 1]);
            if (interFlag) {
                interP = intersectionPoint(S, P[0], W[i], W[i + 1]);
                Q.push_back(interP);
            }
        }
        P = move(Q);
        Q.clear();
        Np = P.size();
    }

    if (!Np) return false;
    res = Polygon(P);
    return true;
}



double lineLen(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

int getNearestPointOnEdge(const vector<Point> &points, const Line &l) {
    vector<int> ans;
    double val;
    for (int i = 0; i < points.size(); ++i) {
        val = l.dist(points[i]);
        if (fabs(val) < EPS && points[i].x > min(l.a.x, l.b.x) + EPS && points[i].x < max(l.a.x, l.b.x) - EPS &&
                               points[i].y > min(l.a.y, l.b.y) + EPS && points[i].y < max(l.a.y, l.b.y) - EPS)
            ans.push_back(i);
    }
    if (ans.empty()) return -1;
    int best_ans = ans[0];
    double best_len = lineLen(points[ans[0]], l.a), len;
    for (auto x: ans) {
        len = lineLen(points[x], l.a);
        if (len < best_len) {
            best_ans = x;
            best_len = len;
        }
    }
    return best_ans;
}

struct node {
    node(int x): val(x), next(nullptr) {}
    node(int x, node *n): val(x), next(n) {}
    int val;
    node* next;
};

Polygon getPolyNodeCircle(node *head, const vector<Point> &pts) {
    vector<Point> ans;
    node *first = head;
    ans.push_back(pts[first->val]);
    head = head->next;
    while (head != first) {
        ans.push_back(pts[head->val]);
        head = head->next;
    }
    return Polygon(ans);
}

vector<Polygon> operator +(const vector<Polygon>&a, const vector<Polygon>&b) {
    vector<Polygon> ans = a;
    for (auto &x: b)
        ans.push_back(x);
    return ans;
}
bool operator == (const Point &a, const Point &b) {
    return fabs(a.x-b.x) < EPS && fabs(a.y-b.y) < EPS;
}

vector<Polygon> dividePolyParse(node *head, const vector<Point> &points, const vector<node*> &nodes) {
    vector<bool> visited(points.size(), false);
    vector<Polygon> ans;
    stack<pair<node*, node*>> later; // next run node, change link
    node* p = head, *next;
    Line e;
    while (!visited[p->val]) {
        visited[p->val] = true;
        e = Line(points[p->val], points[p->next->val]);
        int pt = getNearestPointOnEdge(points, e);
        if (pt >= 0) {
            next = p->next;
            p->next = nodes[pt];
            later.push(make_pair(nodes[pt], next));
            break;
        }
        p = p->next;
    }
    Polygon poly = getPolyNodeCircle(p, points);
    if (poly.area() > EPS)
        ans.push_back(poly);

    while (!later.empty()) {
        later.top().first->next = later.top().second;
        ans = ans + dividePolyParse(later.top().first, points, nodes);
        later.pop();
    }
    return ans;
}

vector<Polygon> dividePoly(const Polygon &poly) {
    vector<Point> points = poly.points;
    vector<node*> nodes(points.size(), nullptr);

    node *head = new node(0), *cur = head;
    nodes[0] = head;
    for (int i = 1; i < points.size(); ++i) {
        cur->next = new node(i);
        cur = cur->next;
        nodes[i] = cur;
    }
    cur->next = head;

   auto ans = dividePolyParse(head, points, nodes);
   for (auto node: nodes)
       delete node;
   return ans;
}





bool cutPolygonDebug(const Polygon &cutter, const Polygon &polygon, Polygon &res, bool init) {
    static Debugger debug;
    if (init) {
        debug.debugIter = -1;
        debug.Pdebug = polygon;
        return false;
    }
    debug.debugIter++;
    if (debug.debugIter >= cutter.points.size())
        return false;

    vector<Point> P = debug.Pdebug.points, Q;
    vector<Point> W = cutter.points;
    W.push_back(W[0]);
    int Np = P.size();
    Point F, S;
    Q.clear();
    int i = debug.debugIter;
    for (int j = 0; j < Np; ++j) {
        if (j == 0)
            F = P[0];
        else {
            bool inter = checkIntersection(S, P[j], W[i], W[i + 1]);
            if (inter) {
                Point interP = intersectionPoint(S, P[j], W[i], W[i + 1]);
                Q.push_back(interP);
            }
        }

        S = P[j];
        char visible = checkVisibility(S, W[i], W[i + 1]);
        if (visible >= 0)
            Q.push_back(S);
    }

    if (!Q.empty()) {
        bool inter = checkIntersection(S, F, W[i], W[i + 1]);
        if (inter) {
            Point interP = intersectionPoint(S, F, W[i], W[i + 1]);
            Q.push_back(interP);
        }
    }
    debug.Pdebug = Polygon(Q);
    Np = Q.size();
    if (!Np) return false;
    res = Polygon(Q);
    return true;
}
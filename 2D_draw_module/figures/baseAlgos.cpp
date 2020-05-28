//
// Created by kurush on 14.02.2020.
//

#include "baseAlgos.h"
#include <cmath>
#include <iostream>

using namespace std;

#define  EPS 1e-5

#define det(a,b,c,d)  (a*d-b*c)

Point getMedian(Line line)
{
    return Point((line.a.x + line.b.x) / 2,
                 (line.a.y + line.b.y) / 2);
}

Point intersectLines(Line m, Line n, bool *ok)
{
    // ok = true only if 1 point
    double zn = det(m.ka, m.kb, n.ka, n.kb);
    if (abs (zn) < EPS)
    {
        if (ok)
            *ok = false;
        return Point();
    }
    else {
        double x = - det (m.kc, m.kb, n.kc, n.kb) / zn;
        double y = - det (m.ka, m.kc, n.ka, n.kc) / zn;
        if (ok)
            *ok = true;
        return Point(x,y);
    }
}
//
// Created by kurush on 11.08.2020.
//

#include "RoamTree.h"

#include <algorithm>
#include <vector>

int cnt = 0;
RoamNode::RoamNode(size_t i1, size_t j1, size_t i2, size_t j2, size_t i3, size_t j3,
         const sptr<QRTexture> &texture, LinkMap &links, const QRMatrix<sptr<QRPoint3D>> &points,
         RoamNode *parent): parent(parent) {
    cnt++;
    triangle = sptr<Triangle3D>(new Triangle3D(points[i1][j1],points[i2][j2],points[i3][j3], texture));
    workPoint = (points[i2][j2]->getVector() + points[i3][j3]->getVector()) / 2;
    size_t iw = (i2+i3)/2, jw = (j2+j3)/2;
    delta = fabs((points[i2][j2]->getVector()[2] + points[i3][j3]->getVector()[2]) / 2 -
             points[iw][jw]->getVector()[2]);

    if (links.find(workPoint) != links.end()) {
        link = links[workPoint];
        links[workPoint]->link = this;

        links.erase(links.find(workPoint));
    }
    else
        links[workPoint] = this;

    if ((i2+i3)%2 || (j2+j3)%2) {
        //cout << i2 << ' ' << i3 << ' ' << j2 << ' ' << j3 << '\n';
        //cout << '\t' << points[i1][j1]->getVector() << ' ' <<
        //points[i2][j2]->getVector() << ' ' << points[i3][j3]->getVector() << '\n';
        return;
    }
    left = new RoamNode(iw, jw, i1, j1, i2, j2,
            texture, links, points, this);
    right = new RoamNode(iw, jw, i3, j3, i1, j1,
            texture, links, points, this);
}

void RoamNode::drawCommandNeigbour(const Vector3D &cam, float d) {
    if (!left)
        mustDraw = 1;
    else mustDraw = 0;
    auto p = parent;
    while(p) {
        p->mustDraw = 2;
        p = p->parent;
    }
    if (left) left->update(cam, d), right->update(cam, d);
}

void RoamNode::update(const Vector3D &cam, float d) {
    if (!left) {
        mustDraw = 1;
        return;
    }
    if (mustDraw == 2) {
        mustDraw = 0;
        left->update(cam, d), right->update(cam, d);
        if (link) link->drawCommandNeigbour(cam, d);
        return;
    }

    float l = delta * delta * ((cam[0] - workPoint[0]) * (cam[0] - workPoint[0]) +
                               (cam[1]-workPoint[1])*(cam[1]-workPoint[1]));
    float r = maxPixelError*maxPixelError /d/d /pixelsPerUnit/pixelsPerUnit
            * pow(((cam[0]-workPoint[0])*(cam[0]-workPoint[0]) +
                                   (cam[1]-workPoint[1])*(cam[1]-workPoint[1]) +
                                   (cam[2]-workPoint[2])*(cam[2]-workPoint[2])), 2);
    mustDraw = (l <= r);
    //cout << (int)mustDraw << ' ' << l << ' ' << r << ' ' << cam[2] << '\n';

    if (mustDraw == 0) {
        if (link) link->drawCommandNeigbour(cam, d);
        left->update(cam, d), right->update(cam, d);
    }
}

int level = 0;
void RoamNode::addPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    if (mustDraw == 2) mustDraw = 0;
    if (mustDraw == 1)
        polygons.push_back(triangle);
    else if (left) {
        level++;
        left->addPolygons(polygons);
        level++;
        right->addPolygons(polygons);
    }
    level--;
}

Frame::Frame(Frame&& f): left(f.left), right(f.right),
center(f.center), radius(f.radius), isVisible(f.isVisible) {
    f.right = nullptr;
    f.left = nullptr;
}

Frame::Frame(const Frame& f): left(f.left), right(f.right),
                         center(f.center), radius(f.radius), isVisible(f.isVisible) {}

Frame::Frame(const QRMatrix<sptr<QRPoint3D>> &points,
      size_t l, size_t r, size_t u, size_t d, LinkMap &links) {
    cnt = 0;
    left = new RoamNode(d, l, d, r, u, l,
            QRTexturesMap[QRDEFAULT_MATERIAL], links, points);
    right = new RoamNode(u, r, u, l, d, r,
                        QRTexturesMap[QRDEFAULT_MATERIAL], links, points);
    // define center
    float zmin = QRINF, zmax = -QRINF;
    for (size_t i = u ; i <= d; ++i)
        for (size_t j = l; j <= r; ++j)
            zmin = min(zmin, points[i][j]->getVector()[2]),
            zmax = max(zmax, points[i][j]->getVector()[2]);

    Vector3D v1 = points[d][l]->getVector();
    Vector3D v2 = points[u][r]->getVector();
    v1[2] = zmin, v2[2] = zmax;
    center = (v1 + v2) / 2;
    radius = max(max(fabs(v1[0]-v2[0])/2, fabs(v1[1]-v2[1])/2), fabs(v1[2]-v2[2])/2);

    /*cout << "links unused:\n";
    std::vector<Vector3D> vec(links.size());
    int i = 0;
    for (auto x: links)
        vec[i++] = x.first;
    std::sort(vec.begin(), vec.end());

    for (auto x: vec)
        cout << x << '\n';*/
}

void Frame::updateCamera(const sptr<QRCamera3D> &camera) {
    // todo hardcode with -origin[2]

    isVisible = camera->isVisibleSphere(center, radius);
    if (isVisible) {
        cout << "UPDATE!!!!!!!!!!!!...........................................\n";
        left->update(camera->getWorldOriginCoord(), -camera->getOrigin()[2]);
        right->update(camera->getWorldOriginCoord(), -camera->getOrigin()[2]);
    }
}

void Frame::addPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    level = 0;
    if (!isVisible) return;
    left->addPolygons(polygons);
    right->addPolygons(polygons);
}
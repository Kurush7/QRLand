//
// Created by kurush on 11.08.2020.
//

#include "RoamTree.h"

#include <algorithm>
#include <vector>

float RoamUpdateConstant = 0;
float CameraCoordsX, CameraCoordsY, CameraCoordsZ;

RoamNode::RoamNode(size_t i1, size_t j1, size_t i2, size_t j2, size_t i3, size_t j3,
         const sptr<QRTexture> &texture, LinkMap &links, const QRMatrix<sptr<QRPoint3D>> &points,
         RoamNode *parent): parent(parent), i1(i1), i2(i2), i3(i3), j1(j1), j2(j2), j3(j3) {
    if (!useIndexedPolygons)
        triangle = sptr<QRPolygon3D>(new Triangle3D(points[i1][j1],points[i2][j2],points[i3][j3], texture));
    else {
        size_t w = points.width();
        triangle = sptr<QRPolygon3D>(new IndexPolygon3D({static_cast<int>(i1*w+j1), // todo wtf here?
                                                         static_cast<int>(i2*w+j2),
                                                         static_cast<int>(i3*w+j3)}, texture, points.getArray()));
    }

    workPoint = (points[i2][j2]->getVector() + points[i3][j3]->getVector()) / 2;
    size_t iw = (i2+i3)/2, jw = (j2+j3)/2;
    delta = fabs((points[i2][j2]->getVector()[2] + points[i3][j3]->getVector()[2]) / 2 -
             points[iw][jw]->getVector()[2]);
    delta *= delta; // in update only its square is used

    if (links.find(workPoint) != links.end()) {
        link = links[workPoint];
        links[workPoint]->link = this;

        links.erase(links.find(workPoint));
    }
    else
        links[workPoint] = this;

    if ((i2+i3)%2 || (j2+j3)%2) {
        return;
    }
    left = new RoamNode(iw, jw, i1, j1, i2, j2,
            texture, links, points, this);
    right = new RoamNode(iw, jw, i3, j3, i1, j1,
            texture, links, points, this);
}

void RoamNode::drawCommandNeigbour() {
    //if (!left) mustDraw = 1;  // todo may be omitted?
    //else mustDraw = 0;
    if (mustDraw == 0 || mustDraw == 2) return;
    mustDraw = 2;
    RoamNode* p = parent;
    while(p && p->mustDraw == 1) {
        p->mustDraw = 2;
        if (p->link)
            p->link->drawCommandNeigbour();
        p = p->parent;
    }
    if (left) left->mustDraw=1, right->mustDraw = 1;
}

void RoamNode::update() {
    if (!left) {
        mustDraw = 1;
        return;
    }
    if (mustDraw == 2)
        mustDraw = 0;
    else {
        float x = ((CameraCoordsX - workPoint[0]) * (CameraCoordsX - workPoint[0]) +
                   (CameraCoordsY - workPoint[1]) * (CameraCoordsY - workPoint[1]));
        float l = delta * x;
        float r = RoamUpdateConstant *
                  (x + (CameraCoordsZ - workPoint[2]) * (CameraCoordsZ - workPoint[2])) *
                  (x + (CameraCoordsZ - workPoint[2]) * (CameraCoordsZ - workPoint[2]));

        mustDraw = l < r;
    }

    if (mustDraw == 0) {
        if (link) link->drawCommandNeigbour();
        left->update(), right->update();
    }
}

void RoamNode::addPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    if (mustDraw == 2) mustDraw = 0;
    if (mustDraw == 1)
        polygons.push_back(triangle);
    else if (left) {
        left->addPolygons(polygons);
        right->addPolygons(polygons);
    }
    mustDraw = 1;
}

void RoamNode::addMaxDetailedPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    if (!left || !right)
        polygons.push_back(triangle);
    else {
        left->addMaxDetailedPolygons(polygons);
        right->addMaxDetailedPolygons(polygons);
    }
}

void RoamNode::getAllPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    polygons.push_back(triangle);
    if (left) {
        left->getAllPolygons(polygons);
        right->getAllPolygons(polygons);
    }
}

void RoamNode::defineShades(const QRVector<bool> &shaded, size_t w) {
    bool is_shaded = false;
    is_shaded = shaded[i1*w + j1] & shaded[i2*w + j2] & shaded[i3*w + j3];
    triangle->setShaded(is_shaded);
    if (left) {
        left->defineShades(shaded, w);
        right->defineShades(shaded, w);
    }
}

void RoamNode::interpolateColors() {
    if (left) {
        left->interpolateColors();
        right->interpolateColors();
        QRColor a = left->triangle->getTexture()->getColor();
        QRColor b = right->triangle->getTexture()->getColor();
        triangle->setTexture(sptr<QRTexture>(new ColorTexture(mixColors(a, b))));
        // todo update: low-level with one texture, these may be enough just to change
    }
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
    // too what if z's are updated?... and they are....
    center = (v1 + v2) / 2;
    radius = vectorLen(v1-v2) / 2;
}

bool Frame::updateCamera(const sptr<QRCamera3D> &camera) {
    isVisible = camera->isVisibleSphere(center, radius);
    if (isVisible) {
        left->update();
        right->update();
        return true;
    }
    return false;
}

void Frame::addPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    if (!isVisible) return;
    left->addPolygons(polygons);
    right->addPolygons(polygons);
}

void Frame::addMaxDetailedPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    left->addMaxDetailedPolygons(polygons);
    right->addMaxDetailedPolygons(polygons);
}

void Frame::getAllPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    left->getAllPolygons(polygons);
    right->getAllPolygons(polygons);
}

void Frame::defineShades(const QRVector<bool> &isShadedPoint, size_t width) {
    left->defineShades(isShadedPoint, width);
    right->defineShades(isShadedPoint, width);
}

void Frame::interpolateColors() {
    left->interpolateColors();
    right->interpolateColors();
}
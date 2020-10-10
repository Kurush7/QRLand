//
// Created by kurush on 11.08.2020.
//

#ifndef BIG3DFLUFFY_ROAMTREE_H
#define BIG3DFLUFFY_ROAMTREE_H

#include "containers/QRContainers.h"
#include "objects/QRObject.h"
#include "objects/composites/impl.h"
#include "objects/components/impl.h"
#include "../basic/QRLandscapeTexture.h"

#include "globalConfig.h"
#include "RoamConfig.h"

struct RoamNode;
using LinkMap = std::map<Vector3D, RoamNode*>;

struct RoamNode {
    sptr<QRPolygon3D> triangle;
    Vector3D workPoint;
    RoamNode *link = nullptr, *left=nullptr, *right=nullptr, *parent=nullptr;
    char mustDraw = 1;     // 0 - no, 1 - yes, 2 - no, came from link
    float delta;
    size_t i1, j1, i2, j2, i3, j3;
    RoamNode() = default;
    // workpoint is one between p2 and p3
    RoamNode(size_t i1, size_t j1, size_t i2, size_t j2, size_t i3, size_t j3,
             const sptr<QRTexture> &texture, LinkMap &links,
             const QRMatrix<sptr<QRPoint3D>> &points, RoamNode *parent=nullptr);

    ~RoamNode() {
        if (left) {
            delete left;
            delete right;
        }
    }

    void update();  // d -- distance from camera to projection screen
    void addPolygons(QRVector<sptr<QRPolygon3D>> &polygons);
    void addMaxDetailedPolygons(QRVector<sptr<QRPolygon3D>> &polygons);
    void getAllPolygons(QRVector<sptr<QRPolygon3D>> &polygons);
    void drawCommandNeigbour();
    void defineShades(const QRVector<bool> &isShadedPoint, size_t width);
    void interpolateColors();

};

struct Frame {
    RoamNode *left = nullptr, *right = nullptr;
    bool isVisible = false;

    // sphere
    Vector3D center;
    float radius;

    Frame() = default;
    Frame(Frame &&f);    // todo !!!!cleans links!!!!
    Frame(const Frame &f);    // todo !!!!cleans links!!!!
    Frame(const QRMatrix<sptr<QRPoint3D>> &points,
            size_t left, size_t right, size_t up, size_t down, LinkMap &links);
    ~Frame() {
        if (left) delete left;
        if (right) delete right;
    }

    void addPolygons(QRVector<sptr<QRPolygon3D>> &polygons);
    void addMaxDetailedPolygons(QRVector<sptr<QRPolygon3D>> &polygons);
    void getAllPolygons(QRVector<sptr<QRPolygon3D>> &polygons);
    bool updateCamera(const sptr<QRCamera3D> &camera);  // returns wether it's visible
    void defineShades(const QRVector<bool> &isShadedPoint, size_t width);
    void interpolateColors();

};


#endif //BIG3DFLUFFY_ROAMTREE_H

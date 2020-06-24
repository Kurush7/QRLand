//
// Created by kurush on 31.05.2020.
//

#include "FrameLoader.h"

Frame3DLoader::Frame3DLoader (sptr<LoadSource> src,  sptr<AbstractObject3DFactory>f)
: source(src), factory(f) {}

bool Frame3DLoader::loadPoints() {
    if (stage != 0) return false;
    int cnt = source->readInt();
    if (!cnt) return false;
    Vector3D cur, center;
    for (int i = 0; i < cnt; ++i) {
        cur[0] = source->readDouble();
        cur[1] = source->readDouble();
        cur[2] = source->readDouble();
        center += cur;
        points.push_back(factory->createPoint(cur));
    }
    center /= cnt;
    for (auto &x: points)
        x->setBind(center);

    stage++;
    return true;
}
bool Frame3DLoader::loadEdges() {
    if (stage != 1) return false;
    int cnt = source->readInt();
    if (!cnt) return false;

    int s, e;
    for (int i = 0; i < cnt; ++i) {
        s = source->readInt();
        e = source->readInt();
        if (s < 0 || s > points.len() || e < 0 || e > points.len())
            throw QRBadSourceException(__FILE__, __LINE__, __TIME__, "corrupted edge index data!");
        objects.push_back(factory->createEdge(points[s]->getPointer(), points[e]->getPointer()));
    }
    for (auto &x: points)
        objects.push_back(x);

    stage++;
    return true;
}

sptr<BaseFrame3D> Frame3DLoader::getFrame() {
    if (!isReady()) return nullptr;
    if (isBuilt) return frame;
    frame = sptr<BaseFrame3D>(new Frame3D(objects));
    isBuilt = true;
    return frame;
}
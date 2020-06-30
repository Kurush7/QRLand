//
// Created by kurush on 30.06.2020.
//

#include "QRender.h"
#include "QRPolyRectCutter.h"

void render (const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene) {
    auto camera = scene->getActiveCamera();
    auto cameraTransformer = camera->getAxisTransformer();
    auto projector = camera->getProjectionTransformer();
    Vector3D screenData = camera->getScreen();
    auto models = scene->getModels();
    for (; models; ++models) {
        auto transformer = models->snd;
        auto model = models->fst;

        // stage 0. pass by out-from view models
        auto center = transformer->transform(ZeroVector);
        if (!camera->isVisibleSphere(center, model->getRadius())) continue;

        // stage 1. set models to scene's coordinates
        // copy points
        QRVector<Vector3D> old_points;
        auto pointsBegin = model->getPoints();
        for (auto it = pointsBegin; it; ++it) {
            old_points.push_back((*it)->getVector());
            // change current vectors: to scene coords, then to camera's
            (*it)->setVector(transformer->transform((*it)->getVector()));
            (*it)->setVector(cameraTransformer->transform((*it)->getVector()));
        }

        // stage 2. delete not front-face polygons
        QRVector<sptr<QRPolygon3D>> front;
        for (auto it = model->getPolygons(); it; ++it) {
            auto poly = *it;
            poly->updateNormal();   // good normal, in camera's coordinates
            if (camera->isFrontFace(poly->getNormal())) front.push_back(poly);
        }
        // stage 3: project into screen  todo z-coord?!!!
        for (auto it = pointsBegin; it; ++it) {
            auto vec = (*it)->getVector();
            vec[3] = 1;
            vec = projector->transform(vec);
            (*it)->setVector(norm(vec));
        }
        // stage 4: render cut polys, then transform to image's coords and draw
        auto mcr = MoveTransformer3DCreator(Vector3D(-screenData[2]/2, screenData[3]/2,0,0));
        auto scr = ScaleTransformer3DCreator(Vector3D(img->getWidth()/screenData[2]/2,
                -img->getHeight()/screenData[3]/2, 1,0));
        auto t = mcr.create();
        t->accumulate(scr.create()->getMatrix());   // todo order?
        for (auto poly: front) {
            RenderPolygon drawPoly = cutPolyRect(poly, screenData);
            for (auto &x: drawPoly)
                x = t->transform(x);    // to image coords
        }
    }
}
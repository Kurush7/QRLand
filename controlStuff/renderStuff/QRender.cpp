//
// Created by kurush on 30.06.2020.
//

#include "QRender.h"
#include "QRPolyRectCutter.h"
#include "QRasterizeZBuffer.h"

void render (const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene) {
    auto camera = scene->getActiveCamera();
    auto cameraTransformer = camera->getAxisTransformer();
    auto projector = camera->getProjectionTransformer();
    Vector3D screenData = camera->getScreen();
    auto models = scene->getModels();

    cout << "camera transformer: " << cameraTransformer->getMatrix() << "\n\n";
    cout << "projector: " << projector->getMatrix() << "\n\n";

    // init zbuffer
    auto zbuf = QRasterizeZBuffer(img, scene->getLights());

    for (; models; ++models) {
        auto transformer = models->snd;
        auto model = models->fst;

        // stage 0. pass by out-from view models
        auto center = transformer->transform(ZeroVector);
        if (!camera->isVisibleSphere(center, model->getRadius())) continue;

        // stage 1. copy points & set model's points to scene's coordinates
        QRVector<Vector3D> old_points;
        auto pointsBegin = model->getPoints();
        auto oldPt = (*pointsBegin)->getVector();
        for (auto it = pointsBegin; it; ++it) {
            old_points.push_back((*it)->getVector());
            // change current vectors: to scene coords, then to camera's
            (*it)->setVector(transformer->transform((*it)->getVector()));   // todo merge into one
            (*it)->setVector(cameraTransformer->transform((*it)->getVector()));
        }

        // stage 2. delete not front-face polygons
        QRVector<sptr<QRPolygon3D>> front;
        // todo fucking hardcode
        auto v = ZeroVector;    // test point for normal redefinition
        v = transformer->transform(v);
        v = cameraTransformer->transform(v);

        for (auto it = model->getPolygons(); it; ++it) {
            auto poly = *it;
            poly->updateNormal();   // good normal, in camera's coordinates
            if (poly->where(v) == FRONT)
                poly->switchNormal();

            if (camera->isFrontFace(poly->getNormal())) front.push_back(poly);
        }
        // stage 3: project points into screen  todo z-coord?!!!
        for (auto it = pointsBegin; it; ++it) {
            auto vec = (*it)->getVector();
            vec[3] = 1;
            //cout << "before projection: " << vec << " => ";
            vec = projector->transform(vec);
            //cout << "after: " << vec << "\n";
            vec = norm(vec);
            (*it)->setVector(norm(vec));
        }

        // stage 4: render cut polys, then transform to image's coords and draw
        auto mcr = MoveTransformer3DCreator(Vector3D(screenData[2]/2, screenData[3]/2,0,0));
        auto scr = ScaleTransformer3DCreator(Vector3D(img->getWidth()/screenData[2],
                img->getHeight()/screenData[3], 1,0));
        auto t = mcr.create();
        t->accumulate(scr.create()->getMatrix());

        cout << "to image: " << t->getMatrix() << "\n\n";
        for (auto poly: front) {

            //cout << "before cut: " << poly << '\n';
            RenderPolygon drawPoly = cutPolyRect(poly, screenData);
            if (drawPoly.getSize() < 3) continue;

            /*cout << "to draw: ";
            for (auto p: drawPoly)
                cout << p << ' ' ;
            cout << '\n';*/

            for (auto &x: drawPoly)
                x = t->transform(x);    // to image coords

            zbuf.draw(drawPoly, poly->getTexture(), poly->getNormal());
        }

        // restore model's points
        int i = 0;
        for (auto it = pointsBegin; it; ++it)
            (*it)->setVector(old_points[i++]);
    }
    cout << "================\n";

    img->repaint();
}
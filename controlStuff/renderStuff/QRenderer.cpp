//
// Created by kurush on 30.06.2020.
//

#include "QRenderer.h"
#include "QRPolyRectCutter.h"
#include "QRasterizeZBuffer.h"

/*TODO
 * 1) застревает при отсечении (иногда)
 * 2) скачок, если сначала мув, потом ротейт
 */

QRenderer::QRenderer(const sptr<QRImage> &image, const sptr<QRPolyScene3D> &scene)
: colorManager(new QRColorManager), zbuf(image, colorManager),
scene(scene.get()), image(image.get()){
    for (auto li=scene->getLights(); li; ++li)
        colorManager->addLight(*li);
}

void QRenderer::render () {
    camera = scene->getActiveCamera().get();
    cameraTransformer = camera->getAxisTransformer().get();
    projector = camera->getProjectionTransformer().get();
    screenData = camera->getScreen();


    auto mcr = MoveTransformer3DCreator(Vector3D(screenData[2]/2, screenData[3]/2,0,0));
    auto scr = ScaleTransformer3DCreator(Vector3D(image->getWidth()/screenData[2],
                                                  image->getHeight()/screenData[3], 1,0));
    imageTransformer = mcr.create().release();
    imageTransformer->accumulate(scr.create()->getMatrix());

    // init zbuffer, fill in black
    zbuf.clearBuf();

    auto models = scene->getModels();
    for (; models; ++models) {
        modelTransformer = models->snd.get();
        model = models->fst.get();

        // stage 0. pass by out-from view models
        auto center = modelTransformer->transform(ZeroVector);
        if (!camera->isVisibleSphere(center, model->getRadius())) continue;

        // todo cut with camera pyramid

        // stage 1. copy points & set model's points to scene's coordinates
        old_points.clear();
        points.clear();
        PointIterator pointsBegin = model->getPoints();
        for (auto it = pointsBegin; it; ++it) {
            old_points.push_back((*it)->getVector());
            points.push_back((*it).get());
            // change current vectors: to scene coords, then to camera's
            (*it)->setVector(modelTransformer->transform((*it)->getVector()));   // todo merge into one
            (*it)->setVector(cameraTransformer->transform((*it)->getVector()));
        }
        points_cnt = points.getSize();

        // stage 2. delete not front-face polygons
        auto transZero = modelTransformer->transform(ZeroVector);   // for normal testing
        transZero = cameraTransformer->transform(transZero);
        PolyPosition vPlace;

        front.clear();
        for (auto it = model->getPolygons(); it; ++it) {
            poly = it->get();
            vPlace = poly->where(ZeroVector);
            poly->updateNormal();   // good normal, in camera's coordinates
            if (poly->where(transZero) != vPlace)
                poly->switchNormal();
            if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) front.push_back(poly);
        }
        front_size = front.getSize();

        // stage 3: project points into screen  todo z-coord: now not controlled. mb scale z's [-1;1]
        for (size_t i = 0; i < points_cnt; ++i) {
            auto vec = points[i]->getVector(); vec[3] = 1;
            vec = projector->transform(vec);
            points[i]->setVector(norm(vec));
        }

        // stage 4: render cut polys, then transform to image's coords and draw
        for (size_t i = 0; i < front_size; ++i) {
            RenderPolygon drawPoly = cutPolyRect(front[i], screenData);
            if (drawPoly.getSize() < 3) continue;

            for (auto &x: drawPoly)      // polygon's points to image coords
                x = imageTransformer->transform(x);

            colorManager->setTexture(front[i]->getTexture());
            zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), front[i]->getNormal());
        }

        // restore model's point
        for (size_t i = 0; i < points_cnt; ++i) {
            points[i]->setVector(old_points[i]);
        }
        for (auto it = model->getPolygons(); it; ++it) {
            poly = it->get();
            vPlace = poly->where(transZero);
            poly->updateNormal();
            if (poly->where(ZeroVector) != vPlace)
                poly->switchNormal();
        }
    }
    image->repaint();
}
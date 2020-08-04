//
// Created by kurush on 30.06.2020.
//

#include "QRenderer.h"
#include "QRPolyRectCutter.h"
#include "QRasterizeZBuffer.h"

#include "tests/TimeTest.h"
#include <memory>

QRenderer::QRenderer(const sptr<QRImage> &image, const sptr<QRPolyScene3D> &scene)
: colorManager(new QRLightManager), zbuf(image, colorManager),
  scene(scene.get()), image(image.get()){
    for (auto li=scene->getLights(); li; ++li)
        colorManager->addLight(*li);
}

void QRenderer::initRender() {
    camera = scene->getActiveCamera().get();
    cameraTransformer = camera->getAxisTransformer().get();
    projector = camera->getProjectionTransformer().get();
    screenData = camera->getScreen();

    auto mcr = MoveTransformer3DCreator(Vector3D(screenData[2]/2, screenData[3]/2,0,0));
    auto scr = ScaleTransformer3DCreator(Vector3D(image->getWidth()/screenData[2],
                                                  image->getHeight()/screenData[3], 1,0));
    // screenData is used in rasterizer, which works already in image coords
    screenData[0] = image->getWidth()/2;
    screenData[1] = image->getHeight()/2;
    screenData[2] = image->getWidth();
    screenData[3] = image->getHeight();

    imageTransformer = mcr.create().release();
    imageTransformer->accumulate(scr.create()->getMatrix());

    // init zbuffer, fill in black
    zbuf.clearBuf();
}

bool QRenderer::modelCameraCut() {
    // todo cut with camera pyramid instead of just this
    // stage 0. pass by out-from view models
    auto center = modelTransformer->transform(ZeroVector);
    if (!camera->isVisibleSphere(center, model->getRadius())) return false;
    return true;
}

void QRenderer::copyTransformPoints() {
    // stage 1. copy points & set model's points to scene's coordinates
    old_vectors.clear();
    points.clear();

    model_pts = model->getPurePoints();
    size_t size = model->getPointCnt();
    for (size_t i = 0; i < size; ++i) {
        old_vectors.push_back(model_pts[i]->getVector());
        points.push_back(model_pts[i].get());
        // change current vectors: to scene coords, then to camera's
        model_pts[i]->setVector(cameraTransformer->transform(
                modelTransformer->transform(model_pts[i]->getVector())));
    }
    points_cnt = points.getSize();
}

void QRenderer::manageFrontFacePolygons() {
    // stage 2. delete not front-face polygons if model is convex

    transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));   // for normal testing

    // todo another dynamic array..... phew!
    activePolys.clear();

    polys = model->getPurePolygons();
    size_t size = model->getPolygonCnt();
    for (size_t i = 0; i < size; ++i) {
        poly = polys[i].get();
        vPlace = poly->where(ZeroVector);
        poly->updateNormal();   // good normal, in camera's coordinates
        if (poly->where(transZero) != vPlace)
            poly->switchNormal();
        if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) activePolys.push_back(poly);
    }

    active_size = activePolys.getSize();
}

void QRenderer::projectPoints() {
    // stage 3: project points into screen  todo z-coord: now not controlled. ?map z's to [-1;1]?
    for (size_t i = 0; i < points_cnt; ++i) {
        auto vec = points[i]->getVector();
        vec[3] = 1;
        vec = projector->transform(vec);
        vec = imageTransformer->transform(vec);
        points[i]->setVector(norm(vec));
    }
}


void QRenderer::threadManagePolygons(QRPolygon3D** polys, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        renderPolygon drawPoly = cutter.cutPolyRect(polys[i]);
        if (drawPoly.getSize() < 3) continue;

        zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), polys[i]->getNormal(),
                  activePolys[i]->getTexture().get());
    }
}

void QRenderer::frameCutDraw() {
    // stage 4: render cut polys, then transform to image's coords and draw
    cutter.setCutter(screenData);

    /*for (size_t i = 0; i < active_size; ++i) {
        renderPolygon drawPoly = cutter.cutPolyRect(activePolys[i]);
        if (drawPoly.getSize() < 3) continue;
        zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), activePolys[i]->getNormal(),
                  activePolys[i]->getTexture().get());
    }
     return;*/

    int thread_cnt = std::thread::hardware_concurrency();
    size_t thread_size = active_size / thread_cnt;
    auto ptr = activePolys.getPureArray();
    thread threads[thread_cnt];
    for (size_t i = 0; i < thread_cnt; ++i) {
        if (i == thread_cnt-1)
            threads[i] = thread(&QRenderer::threadManagePolygons, this, ptr+i*thread_size,
                                thread_size + active_size % thread_cnt);
        else
            threads[i] = thread(&QRenderer::threadManagePolygons, this, ptr+i*thread_size, thread_size);
    }
    for (size_t i = 0; i < thread_cnt; ++i) {
        threads[i].join();
    }
}

void QRenderer::restorePoints() {
    // restore model's points
    for (size_t i = 0; i < points_cnt; ++i) {
        points[i]->setVector(old_vectors[i]);
    }
    polys = model->getPurePolygons();
    size_t size = model->getPolygonCnt();
    for (size_t i = 0; i < size; ++i) {
        poly = polys[i].get();
        vPlace = poly->where(transZero);
        poly->updateNormal();
        if (poly->where(ZeroVector) != vPlace)
            poly->switchNormal();
    }
}

void QRenderer::render () {
    initRender();

    RawModelIterator models = scene->getModels();
    // todo no iterators here
    for (; models; ++models) {
        modelTransformer = models->snd.get();
        model = models->fst.get();

        if (!modelCameraCut()) continue;
        double t = measureTime(bind(&QRenderer::copyTransformPoints, this));
        cout << "copyTransformPoints: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::manageFrontFacePolygons, this));
        cout << "manageFrontFacePolygons: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::projectPoints, this));
        cout << "projectPoints: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::frameCutDraw, this));
        cout << "frameCutDraw: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::restorePoints, this));
        cout << "restorePoints: " << t << " msec\n";

        /*copyTransformPoints();
        manageFrontFacePolygons();
        projectPoints();
        frameCutDraw();
        restorePoints();*/
    }
    system_clock::time_point start = system_clock::now();
    image->repaint();
    system_clock::time_point end = system_clock::now();
    double time = (end - start).count();    // nanosecs
    cout << "render: " << time / 1e6 << " msec\n\n\n";

}
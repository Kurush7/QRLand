//
// Created by kurush on 30.06.2020.
//

#include "QRenderer.h"

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

    model_pts = model->getPurePoints();
    size_t size = model->getPointCnt();
    for (size_t i = 0; i < size; ++i) {
        old_vectors.push_back(model_pts[i]->getVector());
        // change current vectors: to scene coords, then to camera's
        model_pts[i]->setVector(cameraTransformer->transform(
                modelTransformer->transform(model_pts[i]->getVector())));
    }
    points_cnt = model->getPointCnt();
}

void QRenderer::updateNormals() {
    // stage 2. delete not front-face polygons if model is convex

    transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));   // for normal testing

    // todo another dynamic array..... phew!
    //activePolys.clear();

    polys = model->getPurePolygons();
    polys_size = model->getPolygonCnt();
    for (size_t i = 0; i < polys_size; ++i) {
        poly = polys[i].get();
        vPlace = poly->where(ZeroVector);
        poly->updateNormal();   // good normal, in camera's coordinates
        if (poly->where(transZero) != vPlace)
            poly->switchNormal();
        //todo not here if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) activePolys.push_back(poly);
    }
    //active_size = activePolys.getSize();
}

void QRenderer::projectPoints() {
    // stage 3: project points into screen  todo z-coord: now not controlled. ?map z's to [-1;1]?
    for (size_t i = 0; i < points_cnt; ++i) {
        auto vec = model_pts[i]->getVector();
        vec[3] = 1;
        vec = projector->transform(vec);
        vec = imageTransformer->transform(vec);
        model_pts[i]->setVector(norm(vec));
    }
}

mutex print_lock;
void QRenderer::threadManagePolygons(sptr<QRPolygon3D>* polys, size_t size, int step, int thread_num) {
    system_clock::time_point start = system_clock::now();
    size_t skipped=0;
    double cut_time=0, draw_time=0, create_time;
    PolyRectCutter cutter;
    cutter.setCutter(screenData);
    renderPolygon drawPoly;
    for (size_t i = 0, pos=0; i < size; ++i, pos+=step) {
        // todo insert 'i'm invisible from this side of the moon' here
        system_clock::time_point start = system_clock::now();
        cutter.cutPolyRect(polys[pos].get(), drawPoly);
        system_clock::time_point end = system_clock::now();
        cut_time += (end - start).count() / 1e6;
        if (drawPoly.getSize() < 3) {
            skipped++;
            continue;
        }

        start = system_clock::now();
        zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), polys[pos]->getNormal(),
                  polys[pos]->getTexture().get());
        end = system_clock::now();
        draw_time += (end - start).count() / 1e6;
    }
    print_lock.lock();
    cout << "Tread" << thread_num << ":\n\tto draw: " << size;
    cout << "\n\tskipped: " << skipped;
    cout << "\n\ttime to cut: " << cut_time;
    cout << "\n\ttime to draw: " << draw_time;
    cout << "\n\tcreate time: " << create_time;

    cout << "\n\ttotal thread_time:" << (system_clock::now() - start).count() / 1e6;
    cout <<"\n\n";
    print_lock.unlock();
}

void QRenderer::frameCutDraw() {
    // stage 4: render cut polys, then transform to image's coords and draw
    int thread_cnt = thread::hardware_concurrency();
    size_t thread_size = polys_size / thread_cnt;
    auto ptr = model->getPurePolygons();
    thread threads[thread_cnt];
    int remain = polys_size % thread_cnt;
    for (size_t i = 0; i < thread_cnt; ++i) {
        threads[i] = thread(&QRenderer::threadManagePolygons, this, ptr+i,
                            thread_size + (i<remain), thread_cnt, i);
    }

    for (size_t i = 0; i < thread_cnt; ++i) {
        threads[i].join();
    }
}

void QRenderer::restorePoints() {
    // restore model's points
    for (size_t i = 0; i < points_cnt; ++i) {
        model_pts[i]->setVector(old_vectors[i]);
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
    system_clock::time_point start0 = system_clock::now();
    system_clock::time_point start = system_clock::now();
    initRender();
    system_clock::time_point end = system_clock::now();
    double time = (end - start).count() / 1e6;    // nanosecs
    cout << "init render: " << time << " msec\n";

    RawModelIterator models = scene->getModels();
    // todo no iterators here
    for (; models; ++models) {
        modelTransformer = models->snd.get();
        model = models->fst.get();

        if (!modelCameraCut()) continue;
        double t = measureTime(bind(&QRenderer::copyTransformPoints, this));
        cout << "copyTransformPoints: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::updateNormals, this));
        cout << "updateNormals: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::projectPoints, this));
        cout << "projectPoints: " << t << " msec\n";

        t = measureTime(bind(&QRenderer::frameCutDraw, this));
        cout << "frameCutDraw: " << t << " msec\n";
        t = measureTime(bind(&QRenderer::restorePoints, this));
        cout << "restorePoints: " << t << " msec\n";

        /*copyTransformPoints();
        updateNormals();
        projectPoints();
        frameCutDraw();
        restorePoints();*/
    }
    start = system_clock::now();
    image->repaint();
    end = system_clock::now();
    time = (end - start).count() / 1e6;    // nanosecs
    cout << "render: " << time << " msec\n";
    cout << "TOTAL RENDER TIME: " << (system_clock::now() - start0).count() / 1e6;
    cout <<"\n\n\n";

}
//
// Created by kurush on 30.06.2020.
//

#include "QRenderer.h"

QRenderer::QRenderer(const sptr<QRImage> &image, const sptr<QRPolyScene3D> &scene)
: colorManager(new QRLightManager), zbuf(image, colorManager),
  scene(scene.get()), image(image.get()) {
    cutters = new PolyRectCutter[thread_cnt];
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
    // todo transZero for later
    return camera->isVisibleSphere(modelTransformer->transform(ZeroVector),
            model->getRadius());;
}

void QRenderer::copyTransformPoints() {
    // stage 1. copy points & set model's points to scene's coordinates
    model_pts = model->getPurePoints();
    points_cnt = model->getPointCnt();

    old_vectors.clear();
    old_vectors.reserve(points_cnt);
    for (size_t i = 0; i < points_cnt; ++i) {
        old_vectors[i] = model_pts[i]->getVector();
        // change current vectors: to scene coords, then to camera's
        model_pts[i]->setVector(modelCameraTransformer.transform(old_vectors[i]));
    }
}

void QRenderer::updateNormals() {
    // stage 2. delete not front-face polygons if model is convex
    polys = model->getPurePolygons();
    polys_cnt = model->getPolygonCnt();

    normals.clear();
    normals.reserve(polys_cnt);

    transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));   // for normal testing
    transZero[3] = 0;
    for (size_t i = 0; i < polys_cnt; ++i) {
        normals[i] = polys[i]->computeNewPlane();
        if (polys[i]->where(ZeroVector) != sign(scalar(normals[i], transZero) + normals[i][3]))
            normals[i][0] = -normals[i][0], normals[i][1] = -normals[i][1], normals[i][2] = -normals[i][2];
        // normals[i][3] = 1; todo not important since we need only it's xyz
    }
}

void QRenderer::projectPoints() {
    // stage 3: project points into screen  todo z-coord: now not controlled. ?map z's to [-1;1]?
    for (size_t i = 0; i < points_cnt; ++i) {
        auto vec = model_pts[i]->getVector();
        vec[3] = 1;
        cout << "point: " << vec << " -> ";
        vec = norm(projector->transform(vec));  // todo merge into one
        vec = imageTransformer->transform(vec);
        cout << vec << '\n';
        model_pts[i]->setVector(norm(vec));
    }
}

mutex print_lock;
void QRenderer::threadManagePolygons(size_t size, int offset, int step, int thread_num) {
    //todo place it somewhere here
    // if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) draw it
    system_clock::time_point start = system_clock::now();
    size_t skipped=0;
    double cut_time=0, draw_time=0;

    cutters[thread_num].setCutter(screenData);
    renderPolygon drawPoly;

    for (size_t i = 0, pos=offset; i < size; ++i, pos+=step) {
        // todo insert 'i'm invisible from this side of the moon' here
        system_clock::time_point start = system_clock::now();
        cutters[thread_num].cutPolyRect(polys[pos].get(), drawPoly);
        system_clock::time_point end = system_clock::now();

        cut_time += (end - start).count() / 1e6;
        if (drawPoly.getSize() < 3) {
            skipped++;
            continue;
        }

        start = system_clock::now();
        zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), normals[pos],
                  polys[pos]->getTexture().get());
        end = system_clock::now();
        draw_time += (end - start).count() / 1e6;
    }

    print_lock.lock();
    cout << "Tread" << thread_num << ": \ttotal thread_time:" << (system_clock::now() - start).count() / 1e6;
    cout << "\n\t        polys to draw: " << size << " (skipped: " << skipped << ')';
    cout << "\n\t        time to cut: " << cut_time;
    cout << "\n\t        time to draw: " << draw_time;
    cout << "\n\t        total thread_time:" << (system_clock::now() - start).count() / 1e6;
    cout <<"\n\n";
    print_lock.unlock();
}

void QRenderer::frameCutDraw() {
    // stage 4: render cut polys, then transform to image's coords and draw
    screenData[2]--; //todo decrease width&height elsewise after rounding of floats value may overwhelm
    screenData[3]--;

    size_t thread_size = polys_cnt / thread_cnt;
    thread threads[thread_cnt];
    int remain = polys_cnt % thread_cnt;
    for (size_t i = 0; i < thread_cnt; ++i) {
        // threads parse points equally through all matrix-places to avoid idleness when most of scene is cut
        threads[i] = thread(&QRenderer::threadManagePolygons, this, thread_size + (i<remain), i, thread_cnt, i);
    }

    for (size_t i = 0; i < thread_cnt; ++i) {
        threads[i].join();
    }

    system_clock::time_point start = system_clock::now();
    zbuf.fillMissing();
    system_clock::time_point end = system_clock::now();
    cout << "rasterizer errors fixing: " << (end - start).count() / 1e6 << '\n';
}

void QRenderer::restorePoints() {
    // restore model's points
    for (size_t i = 0; i < points_cnt; ++i)
        model_pts[i]->setVector(old_vectors[i]);
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
        modelCameraTransformer = Transformer3D(modelTransformer->getMatrix());
        modelCameraTransformer.accumulate(cameraTransformer->getMatrix());

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
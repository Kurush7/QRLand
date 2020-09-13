//
// Created by kurush on 09.08.2020.
//

#include "QuickRenderer.h"

// TODO НАФИГ В QRENDERDATA СОХРАНЯТЬ XYZ?!

QuickRenderer::QuickRenderer(const sptr<QRImage> &img, QRPolyScene3D *scene)
    : QRenderer(img, scene), zbuf(img, colorManager),
            data(thread_cnt) {
        for (auto li=scene->getLights(); li; ++li)
            colorManager->addLight(*li);

        for (int i = 0; i < thread_cnt; ++i)
            cutters.push_back(sptr<Quick3DCutter>(new Quick3DCutter(*data.data[i])));
}

QuickRenderer::QuickRenderer(const sptr<QRImage> &image, const sptr<QRPolyScene3D> &scene)
        : QRenderer(image, scene), zbuf(image, colorManager),
          data(thread_cnt) {
    for (auto li=scene->getLights(); li; ++li)
        colorManager->addLight(*li);

    for (int i = 0; i < thread_cnt; ++i)
        cutters.push_back(sptr<Quick3DCutter>(new Quick3DCutter(*data.data[i])));
}

void QuickRenderer::initRender() {
    camera = scene->getActiveCamera().get();
    cameraTransformer = camera->getAxisTransformer().get();
    projector = Transformer3D(camera->getProjectionTransformer()->getMatrix());
    screenData = camera->getScreen();

    auto mcr = MoveTransformer3DCreator(Vector3D(screenData[2]/2, 0,0,0));
    auto scr = ScaleTransformer3DCreator(Vector3D(image->getWidth()/screenData[2],
                                                  image->getHeight()/screenData[3], 1,0));

    // screenData is used in rasterizer, which works already in image coords
    screenData[0] = image->getWidth()/2;
    screenData[1] = image->getHeight()/2;
    screenData[2] = image->getWidth();
    screenData[3] = image->getHeight();

    imageTransformer = mcr.create().release();
    imageTransformer->accumulate(scr.create()->getMatrix());
    //colorManager->transformLightsPosition(cameraTransformer);

    //Matrix3D back = inverse(cameraTransformer->getMatrix());
    //colorManager->setTransformFrom(back);

    // init zbuffer, fill in black
    zbuf.clearBuf();
}

bool QuickRenderer::modelCameraCut() {
    // stage 0. pass by out-from view models
    // todo precompute transform of ZeroVector
    return camera->isVisibleSphere(modelTransformer->transform(ZeroVector),
                                   model->getRadius());;
}

mutex print_lock, statistic_lock;
void QuickRenderer::threadDrawPolygons(int thread_num) {
    // todo if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) draw it
    auto dt = data.data[thread_num];
    size_t sz = dt->polygons.getSize();
    for (size_t k = 0; k < sz; ++k) {
        zbuf.draw(dt->points, dt->polygons[k], dt->polygonSize[k],
                  dt->normals[k], textures[dt->rawPolyMap[k]]);
                  //dt->normals[k], polygons[dt->rawPolyMap[k]]->getTexture().get());
    }
}

void QuickRenderer::threadCutPolygons(size_t size, int offset, int step, int thread_num) {
    for (size_t k = 0, pos=offset; k < size; ++k, pos+=step) {
        cutters[thread_num]->cutPoly(pos);
    }
}

void QuickRenderer::getPolygons() {
    // get polygons: ROAM update or just pointer copy
    startMeasureTimeStamp(0);
    model->updateCamera(scene->getActiveCamera(), &polygonData);
    endMeasureTimeIncrement(0);
    startMeasureTimeStamp(1);
    if (!model->isAdditivePolygons()) {
        polygons = model->getPurePolygons();
        polygon_cnt = model->getPolygonCnt();
    }
    else {
        local_polys.clear();
        model->addPolygons(local_polys, &polygonData);
        polygons = local_polys.getPureArray();
        polygon_cnt = local_polys.getSize();
    }
    endMeasureTimeIncrement(1);
}

void QuickRenderer::prepareData() {
    modelCameraTransformer = modelTransformer->getMatrix();
    modelCameraTransformer.accumulate(cameraTransformer->getMatrix());
    transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));
    transZero[3] = 0;

    getPolygons();

    startMeasureTimeStamp(2);
    points = model->getPurePoints();
    point_cnt = model->getPointCnt();
    data.init(modelCameraTransformer.getMatrix(), modelTransformer->getMatrix(),
              points, polygons, point_cnt, polygon_cnt);

    endMeasureTimeIncrement(2);
}

void QuickRenderer::cameraCut() {
    // camera cut & normal updates ... todo separate?
    startMeasureTimeStamp(3);

    auto fr = camera->getFrustrum();
    for (int i = 0; i < thread_cnt; ++i)
        cutters[i]->setCutter(fr.getPureArray(), fr.getSize());

    size_t thread_size = polygon_cnt / thread_cnt;
    thread threads[thread_cnt];
    size_t remain = polygon_cnt % thread_cnt;
    for (size_t i = 0; i < thread_cnt; ++i)
        threads[i] = thread(&QuickRenderer::threadCutPolygons, this,
                            thread_size + (i < remain), i, thread_cnt, i);

    for (size_t i = 0; i < thread_cnt; ++i) threads[i].join();

    endMeasureTimeIncrement(3);

    //polygon_cnt = data.polygons.getSize();
}

void QuickRenderer::project() {
    // point projection
    startMeasureTimeStamp(4);
    data.matrix = imageTransformer->getMatrix();
    data.matrix.addPerspective(projector.getMatrix());
    for (int i = 0; i < thread_cnt; ++i) {
        auto dt = data.data[i];
        for (size_t j = 0; j < dt->pointsSize; ++j)  // todo make one big-matrix multiply
            data.matrix.projMult(dt->myPoints[j]);
    }

    endMeasureTimeIncrement(4);
}

void QuickRenderer::addShades() {
    colorManager->setTransformFrom(modelTransformer->getMatrix());

    textures.reserve(polygon_cnt);
    for (size_t i = 0; i < polygon_cnt; ++i) {
        auto poly = polygons[i].get();
        Vector3D point;
        auto t = poly->getTexture();
        QRColor c = t->getColor();

        bool shaded=true;
        for (auto p = poly->getPointIndexes(); p; ++p) {
            point = points[*p]->getVector();
            shaded &= colorManager->isShaded(point);
        }

        if (shaded)
            colorManager->ambientLight(c);
        textures[i] = c;
    }
}

void QuickRenderer::rasterize() {
    // rasterization
    startMeasureTimeStamp(5);
    //size_t thread_size = polygon_cnt / thread_cnt;
    thread threads[thread_cnt];
    //size_t remain = polygon_cnt % thread_cnt;
    for (size_t i = 0; i < thread_cnt; ++i)
        threads[i] = thread(&QuickRenderer::threadDrawPolygons, this, i);

    for (size_t i = 0; i < thread_cnt; ++i) threads[i].join();
    endMeasureTimeIncrement(5);
}

void QuickRenderer::repaint() {
    startMeasureTimeStamp(6);
    zbuf.fillMissing();
    endMeasureTimeIncrement(6);
    startMeasureTimeStamp(7);
    image->repaint();
    endMeasureTimeIncrement(7);
    endMeasureTimeIncrement(63);
}


void QuickRenderer::render () {
    //return;
    resetAccumulators;
    startMeasureTimeStamp(63);
    initRender();

    RawModelIterator models = scene->getModels();
    polygonData = "";

    for (; models; ++models) {          // todo no iterators here
        model = models->fst.get();
        modelTransformer = models->snd.get();
        if (!modelCameraCut()) continue;

        prepareData();
        cameraCut();
        addShades();
        project();
        rasterize();
    }

    //colorManager->resetLightsPosition();

    repaint();
    printRenderTimes();
}

void QuickRenderer::printRenderTimes() {
    cout << "RENDERING...";
    cout << "\nimage size: w" << image->getWidth() << " * h" << image->getHeight();
    cout << "\n\tmodel: updating camera: " << measureTimeAccumulator(0);
    cout << "\n\tmodel: getting polygons: " << measureTimeAccumulator(1);
    cout << "\n\tdata init: " << measureTimeAccumulator(2);
    cout << "\n\tcamera cut & normal update: " << measureTimeAccumulator(3);
    cout << "\n\tpoint projection: " << measureTimeAccumulator(4);
    cout << "\n\trasterization (4-thread): " << measureTimeAccumulator(5);
    cout << "\n\tpost-processing: " << measureTimeAccumulator(6);
    cout << "\n\trepaint: " << measureTimeAccumulator(7);

    cout << '\n' << polygonData;
    //todo add metric cout << "\n\tpolys camera cut: " << polysCameraCut;
    cout << "\ntotal render time: " << measureTimeAccumulator(63);
    cout << "\n\n\n";
}
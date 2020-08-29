//
// Created by kurush on 09.08.2020.
//

#include "QuickRenderer.h"

QuickRenderer::QuickRenderer(const sptr<QRImage> &image, const sptr<QRPolyScene3D> &scene)
        : QRenderer(image, scene), colorManager(new QRLightManager), zbuf(image, colorManager) {
    cutters = new PolyRectCutter[thread_cnt];
    for (auto li=scene->getLights(); li; ++li)
        colorManager->addLight(*li);
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

    cout << projector.getMatrix() << '\n' << imageTransformer->getMatrix() << "\n***\n";

    projector.accumulate(imageTransformer->getMatrix());

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
void QuickRenderer::threadManagePolygons(size_t size, int offset, int step, int thread_num) {
    // todo if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) draw it
    // todo inits are too slow
    startMeasureTimeStamp(10*thread_num + 0);

    cutters[thread_num].setCutter(screenData);
    renderPolygon drawPoly;

    QRPolygon3D *poly;
    QRVector<Vector3D> points;
    int point_cnt;
    Vector3D normal;

    Poly3DCutter cut3D;     // todo dynamics... away from here
    auto fr = camera->getFrustrum();
    cut3D.setCutter(fr.getPureArray(), fr.getSize());

    sptr<QRPoint3D>* poly_pts;      // todo same as above
    points.reserve(maxPolygonPointcnt);
    for (size_t k = 0, pos=offset; k < size; ++k, pos+=step) {
        poly = polygons[pos].get();
        poly_pts = poly->getPurePoints();
        point_cnt = poly->getSize();

        // copy & camera-transform points
        startMeasureTimeStamp(10*thread_num + 1);
        for (int i = 0; i < point_cnt; ++i)
            points[i] = modelCameraTransformer.transform(poly_pts[i]->getVector());
        endMeasureTimeIncrement(10*thread_num+1);

        // camera pyramid's cutting
        startMeasureTimeStamp(10*thread_num + 2);
        cutResult res = cut3D.cutPoly(points.getPureArray(), point_cnt, drawPoly);
        if (res == CUT_EMPTY) {
            statistic_lock.lock();
            polysCameraCut++;
            statistic_lock.unlock();
            continue;
        }
        // todo not elegant... maybe capture raw array here
        /*point_cnt = drawPoly.getSize();
        for (int i = 0; i < point_cnt; ++i)
            points[i] = drawPoly[i];*/

        endMeasureTimeIncrement(10*thread_num+2);

        // normal update
        startMeasureTimeStamp(10*thread_num + 3);
        normal = lenNorm((points[1] - points[0]) * (points[2] - points[1]));
        normal[3] = -scalar(normal, points[0]);
        if (poly->where(ZeroVector) != sign(scalar(normal, transZero) + normal[3]))
            normal[0] = -normal[0], normal[1] = -normal[1], normal[2] = -normal[2];
        endMeasureTimeIncrement(10*thread_num+3);

        // point projection
        startMeasureTimeStamp(10*thread_num + 4);
        for (int i = 0; i < point_cnt; ++i) {
            points[i][3] = 1;
            points[i] = norm(projector.transform(points[i]));  // todo merge into one
            points[i] = imageTransformer->transform(points[i]);
        }
        endMeasureTimeIncrement(10*thread_num+4);

        // 2d-frame cutting
        startMeasureTimeStamp(10*thread_num + 5);
        cutters[thread_num].cutPolyRect(points.getPureArray(), point_cnt, drawPoly);
        if (drawPoly.getSize() < 3) {
            statistic_lock.lock();
            polysFrameCut++;
            statistic_lock.unlock();
            continue;
        }
        endMeasureTimeIncrement(10*thread_num+5);

        // rasterization
        startMeasureTimeStamp(10*thread_num + 6);
        //zbuf.draw(points.getPureArray(), point_cnt, normal,
        //          poly->getTexture().get());
        zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), normal,
                  poly->getTexture().get());
        endMeasureTimeIncrement(10*thread_num+6);
    }

    print_lock.lock();
    cout << "\n\tTHREAD #" << thread_num;
    cout << "\n\t\tmodel-camera transform: " << measureTimeAccumulator(10*thread_num + 1);
    cout << "\n\t\tcamera cutting: " << measureTimeAccumulator(10*thread_num + 2);
    cout << "\n\t\tnormal update: " << measureTimeAccumulator(10*thread_num + 3);
    cout << "\n\t\tpoint projection: " << measureTimeAccumulator(10*thread_num + 4);
    cout << "\n\t\t2d-frame cutting: " << measureTimeAccumulator(10*thread_num + 5);
    cout << "\n\t\trasterization: " << measureTimeAccumulator(10*thread_num + 6);
    cout << "\n\ttotal thread work: " << endMeasureTimeValue(10*thread_num+0);
    print_lock.unlock();

}

void QuickRenderer::render () {
    resetAccumulators;
    cout << "RENDERING...";
    startMeasureTimeStamp(63);
    initRender();

    RawModelIterator models = scene->getModels();
    string polygonData;
    polysCameraCut=0, polysFrameCut=0;

    for (; models; ++models) {          // todo no iterators here
        model = models->fst.get();
        modelTransformer = models->snd.get();
        if (!modelCameraCut()) continue;

        // todo overload = operator
        modelCameraTransformer = Transformer3D(modelTransformer->getMatrix());
        modelCameraTransformer.accumulate(cameraTransformer->getMatrix());

        startMeasureTime;
        model->updateCamera(scene->getActiveCamera(), &polygonData);
        cout << "\n\tcamera update: " << endMeasureTime;
        startMeasureTime;
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
        cout << "\n\tpolygon getting: " << endMeasureTime << '\n';

        points = model->getPurePoints();
        point_cnt = model->getPointCnt();

        transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));
        transZero[3] = 0;

        quickData.init(polygon_cnt, point_cnt, modelCameraTransformer.getMatrix());
        // todo send 2 qrvectors to init
        quickData.raw_points = points;
        quickData.raw_point_cnt = point_cnt;
        quickData.raw_polygons = polygons;
        quickData.raw_polygon_cnt = polygon_cnt;

        Quick3DCutter cutter(quickData);
        auto fr = camera->getFrustrum();
        cutter.setCutter(fr.getPureArray(), fr.getSize());
        for (size_t i = 0; i < polygon_cnt; ++i)
            cutter.cutPoly(i);

        polygon_cnt = quickData.polygons.getSize();


        quickData.matrix = projector.getMatrix();
        for (size_t i = 0; i < quickData.pointsSize; ++i)
            quickData.matrix.mult(quickData.points[i]);

        for (size_t i = 0; i < quickData.polygons.getSize(); ++i) {
            zbuf.draw(quickData.points, quickData.polygons[i], quickData.polygonSize[i], quickData.normals[i],
                      polygons[quickData.rawPolyMap[i]]->getTexture().get());
        }


        /*size_t thread_size = polygon_cnt / thread_cnt;
        thread threads[thread_cnt];
        size_t remain = polygon_cnt % thread_cnt;

        screenData[2]-=2;
        screenData[3]-=2;

        startMeasureTime;
        for (size_t i = 0; i < thread_cnt; ++i)
            threads[i] = thread(&QuickRenderer::threadManagePolygons, this,
                    thread_size + (i < remain), i, thread_cnt, i);

        for (size_t i = 0; i < thread_cnt; ++i) threads[i].join();
        cout << "\n\tthreads joined in: " << endMeasureTime << '\n';*/

    }

    startMeasureTime;
    zbuf.fillMissing();
    cout << "\n\trasterizer errors polishing: " << endMeasureTime;
    startMeasureTime;
    image->repaint();
    cout << "\n\timage repaint: " << endMeasureTime;
    cout << "\ntotal render time: " << endMeasureTimeValue(63);

    cout << '\n' << polygonData;
    cout << "\n\tpolys camera cut: " << polysCameraCut;
    cout << "\n\tpolys 2d-frame cut: " << polysFrameCut;
    cout << "\nRENDER FINISHED\n\n\n";
}
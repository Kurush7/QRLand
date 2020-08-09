//
// Created by kurush on 09.08.2020.
//

#include "FullThreadRenderer.h"

FullThreadRenderer::FullThreadRenderer(const sptr<QRImage> &image, const sptr<QRPolyScene3D> &scene)
        : QRenderer(image, scene), colorManager(new QRLightManager), zbuf(image, colorManager) {
    cutters = new PolyRectCutter[thread_cnt];
    for (auto li=scene->getLights(); li; ++li)
        colorManager->addLight(*li);
}

void FullThreadRenderer::initRender() {
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

bool FullThreadRenderer::modelCameraCut() {
    // todo cut with camera pyramid instead of just this
    // stage 0. pass by out-from view models
    // todo transZero for later
    return camera->isVisibleSphere(modelTransformer->transform(ZeroVector),
                                   model->getRadius());;
}

mutex print_lock;
void FullThreadRenderer::threadManagePolygons(size_t size, int offset, int step, int thread_num) {
    // if (!model->isConvex() || camera->isFrontFace(poly->getNormal())) draw it

    cutters[thread_num].setCutter(screenData);
    renderPolygon drawPoly;

    QRPolygon3D *poly;
    QRVector<Vector3D> points;
    int point_cnt;

    sptr<QRPoint3D>* poly_pts;

    for (size_t i = 0, pos=offset; i < size; ++i, pos+=step) {
        // todo insert 'i'm invisible from this side of the moon' here
        poly = polygons[pos].get();
        poly_pts = poly->getPurePoints();
        point_cnt = poly->getSize();

        // copy & camera-transform points
        points.reserve(point_cnt);
        for (int i = 0; i < point_cnt; ++i)
            points[i] = modelCameraTransformer.transform(poly_pts[i]->getVector());

        // todo pyramid cutting here

        // normal update
        Vector3D normal = lenNorm((points[1] - points[0]) * (points[2] - points[1]));
        normal[3] = -scalar(normal, points[0]);
        if (poly->where(ZeroVector) != sign(scalar(normal, transZero) + normal[3]))
            normal[0] = -normal[0], normal[1] = -normal[1], normal[2] = -normal[2];

        // point projection
        for (int i = 0; i < point_cnt; ++i) {
            points[i][3] = 1;
            points[i] = norm(projector->transform(points[i]));  // todo merge into one
            points[i] = imageTransformer->transform(points[i]);
        }

        // 2d-frame cutting
        cutters[thread_num].cutPolyRect(points.getPureArray(), point_cnt, drawPoly);
        if (drawPoly.getSize() < 3) continue;

        // rasterization
        zbuf.draw(drawPoly.getPureArray(), drawPoly.getSize(), normal,
                  poly->getTexture().get());
    }

}

void FullThreadRenderer::render () {
    initRender();

    RawModelIterator models = scene->getModels();

    for (; models; ++models) {          // todo no iterators here
        modelTransformer = models->snd.get();
        modelCameraTransformer = Transformer3D(modelTransformer->getMatrix());
        modelCameraTransformer.accumulate(cameraTransformer->getMatrix());

        model = models->fst.get();

        //if (!modelCameraCut()) continue; // todo more here

        polygons = model->getPurePolygons();
        size_t polygon_cnt = model->getPolygonCnt();

        transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));
        transZero[3] = 0;

        size_t thread_size = polygon_cnt / thread_cnt;
        thread threads[thread_cnt];
        size_t remain = polygon_cnt % thread_cnt;

        for (size_t i = 0; i < thread_cnt; ++i)
            threads[i] = thread(&FullThreadRenderer::threadManagePolygons, this,
                    thread_size + (i < remain), i, thread_cnt, i);

        for (size_t i = 0; i < thread_cnt; ++i) threads[i].join();

    }
    zbuf.fillMissing();
    image->repaint();
}
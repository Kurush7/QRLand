//
// Created by kurush on 10.09.2020.
//

#include "QuickShadowRenderer.h"

QuickShadowRenderer::QuickShadowRenderer(sptr<QuickRenderer> &r, int light_source_pos)
:image(new FakeImage(r->getImage()->getWidth(), r->getImage()->getHeight())),
light_source_pos(light_source_pos), renderer(r) {}

void QuickShadowRenderer::initRender() {
    auto light = renderer->colorManager->getLight(light_source_pos);
    Vector3D lightDir = light->getLightVector(), ox, oy;
    if (fabs(lightDir[2]) < QREPS) ox = ZVector;
    else ox = Vector3D(1, 1, (-lightDir[0]-lightDir[1]) / lightDir[2]);
    oy = lenNorm(ox*lightDir);  // todo left, right......

    cout << "ox: " << ox << "\noy: " << oy << "\noz: " << lightDir << '\n';

    renderer->cameraTransformer = new AxisChangeTransformer(ox, oy, lightDir, ZeroVector);    // todo delete it!

    // todo MUST NOT CLEAR IMAGE: NO DATA THERE!!!
    renderer->zbuf.clearZBufOnly();
}

void QuickShadowRenderer::prepareData() {
    renderer->modelCameraTransformer = renderer->modelTransformer->getMatrix();
    renderer->modelCameraTransformer.accumulate(renderer->cameraTransformer->getMatrix());
    renderer->transZero = renderer->cameraTransformer->transform(renderer->modelTransformer->transform(ZeroVector));
    renderer->transZero[3] = 0;

    if (!renderer->model->isAdditivePolygons()) {
        renderer->polygons = renderer->model->getPurePolygons();
        renderer->polygon_cnt = renderer->model->getPolygonCnt();
    }
    else {
        renderer->local_polys.clear();
        renderer->model->addMaxDetailPolygons(renderer->local_polys, &renderer->polygonData);
        renderer->polygons = renderer->local_polys.getPureArray();
        renderer->polygon_cnt = renderer->local_polys.getSize();
    }

    cout << "all polygons count: " << renderer->polygon_cnt << '\n';

    startMeasureTimeStamp(2);
    renderer->points = renderer->model->getPurePoints();
    renderer->point_cnt = renderer->model->getPointCnt();
    renderer->data.init(renderer->modelCameraTransformer.getMatrix(), renderer->modelTransformer->getMatrix(),
                        renderer->points, renderer->polygons, renderer->point_cnt, renderer->polygon_cnt);

    endMeasureTimeIncrement(2);
}

void QuickShadowRenderer::rasterize() {
    startMeasureTimeStamp(5);

    /*size_t thread_size = polygon_cnt / thread_cnt;
    thread threads[renderer->thread_cnt];
    //size_t remain = polygon_cnt % thread_cnt;
    for (size_t i = 0; i < renderer->thread_cnt; ++i)
        threads[i] = thread(&QuickRenderer::threadDrawPolygons, renderer.get(), i);

    for (size_t i = 0; i < renderer->thread_cnt; ++i) threads[i].join();*/
    renderer->threadDrawPolygons(0);

     endMeasureTimeIncrement(5);
}

mutex dataShadowMutex;
void QuickShadowRenderer::threadTransformPoints(size_t size, int offset, int step, int thread_num) {
    auto data = renderer->data.data[thread_num];
    int c;
    QRVector<int32_t> P;
    P.reserve(100);
    size_t x;
    QRVector<int32_t> &pointCodes = data->pointCodes;
    for (size_t k = 0, pos=offset; k < size; ++k, pos+=step) {
        auto indexes = data->raw_polygons[pos]->getPurePointIndexes();
        int sz = data->raw_polygons[pos]->getSize();
        P.setSize(sz);
        for (int i = 0; i < sz; ++i) {
            c = pointCodes[indexes[i]];
            if (c == 0) {
                x = data->addRawPoint(indexes[i]);
                data->matrix.mult(data->points[x]);
                dataShadowMutex.lock();
                pointCodes[indexes[i]] = x;
                dataShadowMutex.unlock();
                P[i] = x;
            }
            else {
                P[i] = c;
            }
        }
        data->addPoly(P.getPureArray(), P.getSize(), pos);
    }
}

void QuickShadowRenderer::transformPoints() {
    /*size_t thread_size = renderer->polygon_cnt / renderer->thread_cnt;
    thread threads[renderer->thread_cnt];
    size_t remain = renderer->polygon_cnt % renderer->thread_cnt;
    for (size_t i = 0; i < renderer->thread_cnt; ++i)
        threads[i] = thread(&QuickShadowRenderer::threadTransformPoints, this,
                            thread_size + (i < remain), i, renderer->thread_cnt, i);

    for (size_t i = 0; i < renderer->thread_cnt; ++i) threads[i].join();*/
    threadTransformPoints(renderer->polygon_cnt, 0, 1, 0);

    float l=1e9,r=-1e9,u=-1e9,d=1e9;
    for (int i = 0; i < renderer->thread_cnt; ++i) {
        auto dt = renderer->data.data[i];
        cout << dt->pointsSize << '\n';
        for (size_t j = 0; j < dt->pointsSize; ++j) {
            l = min(l, dt->myPoints[j][0]);
            r = max(r, dt->myPoints[j][0]);
            u = max(u, dt->myPoints[j][1]);
            d = min(d, dt->myPoints[j][1]);
        }
    }

    l -= 1, r += 1, u += 1, d -= 1;     // todo just in case....

    renderer->screenData[0] = (l+r)/2;
    renderer->screenData[1] = (u+d)/2;
    // todo if render it: some data extends over limits!!!
    renderer->screenData[2] = r-l;
    renderer->screenData[3] = u-d;
    //renderer->screenData[2] = 2*max(fabs(r),fabs(l));
    //renderer->screenData[3] = 2*max(fabs(u), fabs(d));
    cout << l << ' ' << r << " <=> " << u << ' ' <<  d<< '\n';
    auto mcr = MoveTransformer3DCreator(Vector3D(-l, -d,0));
    auto scr = ScaleTransformer3DCreator(Vector3D(image->getWidth()/renderer->screenData[2],
                                                  image->getHeight()/renderer->screenData[3], 1,0));

    // screenData is used in rasterizer, which works already in image coords
    renderer->screenData[0] = image->getWidth()/2;
    renderer->screenData[1] = image->getHeight()/2;
    renderer->screenData[2] = image->getWidth();
    renderer->screenData[3] = image->getHeight();

    renderer->imageTransformer = mcr.create().release();
    renderer->imageTransformer->accumulate(scr.create()->getMatrix());
    renderer->data.matrix = renderer->imageTransformer->getMatrix();




    auto dt = renderer->data.data[0];
    for (size_t j = 0; j < dt->pointsSize; ++j) {
        renderer->data.matrix.mult(dt->myPoints[j]);
    }

}

void QuickShadowRenderer::render() {
    initRender();

    RawModelIterator models = renderer->scene->getModels();
    for (; models; ++models) {
        renderer->model = models->fst.get();
        renderer->modelTransformer = models->snd.get();

        prepareData();

        transformPoints();

        cout << renderer->imageTransformer->getMatrix() << '\n';
        cout << renderer->cameraTransformer->getMatrix();

        rasterize();
    }

    auto man = renderer->getColorManager();
    man->useShades(true);
    int w = renderer->zbuf.getW(), h = renderer->zbuf.getH();
    man->setShadesZBuf(renderer->zbuf.getZBuf(), w, h, light_source_pos);
    Matrix3D to = renderer->imageTransformer->getMatrix() * renderer->cameraTransformer->getMatrix();
    man->setTransformTo(to);

    // todo this fuck works only if there's only one model to render, and it's ROAM-landscape one
    man->setTransformFrom(renderer->modelTransformer->getMatrix());
    auto points = renderer->points;
    size_t size = renderer->point_cnt;
    QRVector<bool> visiblePoints(size);
    for (size_t i = 0; i < size; ++i)
        visiblePoints[i] = renderer->colorManager->isShaded(points[i]->getVector());
    renderer->model->defineShades(visiblePoints);
    //renderer->repaint();
}
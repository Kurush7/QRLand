//
// Created by kurush on 10.09.2020.
//

#include "QuickShadowRenderer.h"

QuickShadowRenderer::QuickShadowRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene, int light_source_pos)
:QuickRenderer(sptr<QRImage>(new FakeImage(img->getWidth(), img->getHeight())), scene),
light_source_pos(light_source_pos) {}

void QuickShadowRenderer::initRender() {
    auto light = colorManager->getLight(light_source_pos);
    Vector3D lightDir = light->getLightVector(), ox, oy;
    if (fabs(lightDir[2]) < QREPS) ox = ZVector;
    else ox = Vector3D(1, 1, (-lightDir[0]-lightDir[1]) / lightDir[2]);
    oy = lenNorm(ox*lightDir);  // todo left, right......
    cameraTransformer = new AxisChangeTransformer(ox, oy, lightDir, ZeroVector);    // todo delete it!

    // todo MUST NOT CLEAR IMAGE: NO DATA THERE!!!
    zbuf.clearZBufOnly();
}

void QuickShadowRenderer::prepareData() {
    modelCameraTransformer = modelTransformer->getMatrix();
    modelCameraTransformer.accumulate(cameraTransformer->getMatrix());
    transZero = cameraTransformer->transform(modelTransformer->transform(ZeroVector));
    transZero[3] = 0;

    getPolygons();

    startMeasureTimeStamp(2);
    points = model->getPurePoints();
    point_cnt = model->getPointCnt();

    endMeasureTimeIncrement(2);
}

void QuickShadowRenderer::rasterize() {
    startMeasureTimeStamp(5);
    //size_t thread_size = polygon_cnt / thread_cnt;
    thread threads[thread_cnt];
    //size_t remain = polygon_cnt % thread_cnt;
    for (size_t i = 0; i < thread_cnt; ++i)
        threads[i] = thread(&QuickShadowRenderer::threadDrawPolygons, this, i);

    for (size_t i = 0; i < thread_cnt; ++i) threads[i].join();
    endMeasureTimeIncrement(5);
}

mutex dataShadowMutex;
void QuickShadowRenderer::threadTransformPoints(size_t size, int offset, int step, int thread_num) {
    auto data = *(this->data.data[thread_num]);
    int c;
    QRVector<int32_t> P;
    P.reserve(100);
    size_t x;
    QRVector<int32_t> &pointCodes = data.pointCodes;
    for (size_t k = 0, pos=offset; k < size; ++k, pos+=step) {
        auto indexes = data.raw_polygons[pos]->getPurePointIndexes();
        int sz = data.raw_polygons[pos]->getSize();
        P.setSize(sz);
        for (int i = 0; i < sz; ++i) {
            c = pointCodes[indexes[i]];
            if (c == 0) {
                x = data.addRawPoint(indexes[i]);
                data.matrix.mult(data.points[x]);
                dataShadowMutex.lock();
                pointCodes[indexes[i]] = x;
                dataShadowMutex.unlock();
                P[i] = x;
            }
            else {
                P[i] = c;
            }
        }
        data.addPoly(P.getPureArray(), P.getSize(), pos);
    }
}

void QuickShadowRenderer::transformPoints() {
    size_t thread_size = polygon_cnt / thread_cnt;
    thread threads[thread_cnt];
    size_t remain = polygon_cnt % thread_cnt;
    for (size_t i = 0; i < thread_cnt; ++i)
        threads[i] = thread(&QuickShadowRenderer::threadTransformPoints, this,
                            thread_size + (i < remain), i, thread_cnt, i);

    for (size_t i = 0; i < thread_cnt; ++i) threads[i].join();

    float l=1e9,r=-1e9,u=1e9,d=-1e9;
    for (int i = 0; i < thread_cnt; ++i) {
        auto dt = data.data[i];
        for (size_t j = 0; j < dt->pointsSize; ++j) {
            l = min(l, dt->myPoints[j][0]);
            r = max(r, dt->myPoints[j][0]);
            u = max(u, dt->myPoints[j][1]);
            d = min(d, dt->myPoints[j][1]);
        }
    }

    l -= 1, r += 1, u += 1, d -= 1;     // todo just in case....

    screenData[0] = (l+r)/2;
    screenData[1] = (u+d)/2;
    screenData[2] = r-l;
    screenData[3] = u-d;
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
    data.matrix = imageTransformer->getMatrix();
    data.matrix.addPerspective(projector.getMatrix());
    for (int i = 0; i < thread_cnt; ++i) {
        auto dt = data.data[i];
        for (size_t j = 0; j < dt->pointsSize; ++j)
            data.matrix.projMult(dt->myPoints[j]);
    }

}

void QuickShadowRenderer::render() {
    initRender();

    RawModelIterator models = scene->getModels();
    for (; models; ++models) {
        model = models->fst.get();
        modelTransformer = models->snd.get();

        transformPoints();
        rasterize();
    }
}
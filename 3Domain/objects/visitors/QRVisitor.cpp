//
// Created by kurush on 24.06.2020.
//

/*
 FOR FAST TEMPLATING
void visitPoint3D(sptr<BaseQRPoint3D> point) {

}
void visitEdge3D(sptr<BaseEdge3D> edge) {

}
void visitCamera3D(sptr<BaseCamera3D> camera) {

}
void visitFrame3D(sptr<BaseFrame3D> frame) {

}
 */

#include "QRVisitor.h"

// todo camera visitors are fucked up after changing get/setOrigin interface

void QRVisitor::visitFrame3D(sptr<QRFrame3D> frame) {
    for (auto obj: frame->getObjects())
        obj->acceptVisitor(this->p);
}

void TransformVisitor::visitPoint3D(sptr<QRPoint3D> point) {
    auto vec = point->getRelativeVector();
    vec = transformer->transform(vec);
    point->setRelativeVector(vec);
}
void TransformVisitor::visitEdge3D(sptr<QREdge3D> edge) {}
void TransformVisitor::visitCamera3D(sptr<QRCamera3D> camera) {
    // todo fuck camera->getOrigin()->acceptVisitor(p);
}
void TransformVisitor::visitFrame3D(sptr<QRFrame3D> frame) {
    for (auto x: frame->getObjects())
        x->acceptVisitor(p);

}

void MoveTransformVisitor::visitPoint3D(sptr<QRPoint3D> point) {
    auto vec = point->getRelativeVector();
    vec = transformer->transform(vec);
    point->setRelativeVector(vec);
    frameBind = point->getBind();
}

void MoveTransformVisitor::visitFrame3D(sptr<QRFrame3D> frame) {
    for (auto x: frame->getObjects())
        x->acceptVisitor(p);

    //set new bind for frame
    frameBind = transformer->transform(frameBind);
    auto vis = sptr<QRVisitor>(new BindSetterVisitor(frameBind));
    frame->acceptVisitor(vis);
}

void BindSetterVisitor::visitPoint3D(sptr<QRPoint3D> point) {
    point->setBind(bind);
}
void BindSetterVisitor::visitEdge3D(sptr<QREdge3D> edge) {}
void BindSetterVisitor::visitCamera3D(sptr<QRCamera3D> camera) {
    //camera->getOrigin()->setBind(bind);
}


void ScaleCameraVisitor::visitCamera3D(sptr<QRCamera3D> camera) {
    //camera->getOrigin()->acceptVisitor(p);
    auto w = camera->getWidth(), h = camera->getHeight();
    Vector3D vec(w, h, 0);
    vec = transformer->transform(vec);
    camera->setWidth(vec[0]);
    camera->setHeight(vec[1]);
}


void SelectionVisitor::visitPoint3D(sptr<QRPoint3D> point) {}
void SelectionVisitor::visitEdge3D(sptr<QREdge3D> edge) {
    auto s = transformer->transform(edge->getStart()->getVector());
    auto e = transformer->transform(edge->getEnd()->getVector());

    float a = e[1] - s[1];
    float b = s[0] - e[0];
    float c = -s[0]*a - s[1]*b;
    float len_ab = sqrt(a*a+b*b);
    float len = (x*a + y*b + c) / len_ab;   // from full line to point: perpendicular

    float a1 = a/len_ab, b1 = b/len_ab;    // a1b1 - normal with len 1
    a1 *= len, b1 *= len;
    float x1 = x + a1, y1 = y + b1;    // coords of point projection on line

    float len2 = (x1*a + y1*b + c) / len_ab;   // distance from line. we could've chosen wrong normal
    if (fabs(len2) > QREPS)
        x1 = x - a1, y1 = y - b1;

    float dx1 = x1 - s[0], dy1 = y1 - s[1];    // vectors with edge points
    float dx2 = x1 - e[0], dy2 = y1 - e[1];

    if (dx1 * dx2 > -QREPS && dy1 * dy2 > - QREPS) {    // point is outside 'edge-rectangle'
        float len1 = sqrt(dx1*dx1  + dy1*dy1);
        len2 = sqrt(dx2*dx2  + dy2*dy2);
        len1 = min(len1,len2);
        len = sqrt(len * len + len1*len1);
    }

    float val = sqrt(x*x+y*y);
    is_selected |= (fabs(len) + val)/ val < SELECTION_ERROR;
    //cout << "edge " << s[0] << ' ' << s[1] << " <=> " << e[0] << ' ' << e[1] << " :: ";
    //cout << len << ' ' << val << ' ' << (fabs(len) + val)/ val << '\n';
}
void SelectionVisitor::visitCamera3D(sptr<QRCamera3D> camera) {
    is_selected = false;
}


void SetColorVisitor::visitPoint3D(sptr<QRPoint3D> point) {
    point->setStyle(keeper.pointStyle);
}
void SetColorVisitor::visitEdge3D(sptr<QREdge3D> edge) {
    edge->setStyle(keeper.edgeStyle);
}
void SetColorVisitor::visitCamera3D(sptr<QRCamera3D> camera) {}

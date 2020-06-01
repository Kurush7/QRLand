//
// Created by kurush on 31.05.2020.
//

#include "Visitor.h"
/*
 FOR FAST TEMPLATING
void visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {

}
void visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {

}
void visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {

}
void visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {

}
 */


void Visitor::visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {
    for (auto obj: frame->getObjects())
        obj->acceptVisitor(this->p);
}

void DrawablePoint::acceptVisitor(std::shared_ptr<BaseDrawMethodVisitor> &v) {v->visitDrawPoint(*this);}
void DrawableEdge::acceptVisitor(std::shared_ptr<BaseDrawMethodVisitor> &v) {v->visitDrawEdge(*this);}

void DrawVisitor::visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {
    auto p = point->getPoint();
    p = transformer->transform(p);
    //p = norm(p);
    auto style = point->isSelected()? painter->getSelectionPointStyle() : point->getStyle();
    data.push_front(std::shared_ptr<DrawableData>(new DrawablePoint(p[0], p[1], p[2], style)));
}
void DrawVisitor::visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {
    auto pointer = edge->getStart();
    auto ps = norm(transformer->transform(pointer->getPoint()));
    pointer = edge->getEnd();
    auto pe = norm(transformer->transform(pointer->getPoint()));
    auto style = edge->isSelected()? painter->getSelectionEdgeStyle() : edge->getStyle();
    data.push_back(std::shared_ptr<DrawableData>(new DrawableEdge(ps[0], ps[1], ps[2],
            pe[0], pe[1], pe[2], style)));
}
void DrawVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {}


void TransformVisitor::visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {
    auto vec = point->getRelativePoint();
    vec = transformer->transform(vec);
    point->setRelativeVector(vec);
    frameBind = point->getBind();
}
void TransformVisitor::visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {
    edge->getStart()->acceptVisitor(p);
    edge->getEnd()->acceptVisitor(p);
}
void TransformVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {
    camera->getOrigin()->acceptVisitor(p);
}
void TransformVisitor::visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {
    for (auto x: frame->getObjects())
        x->acceptVisitor(p);

    //set new bind for frame
    frameBind = transformer->transform(frameBind);
    auto vis = shared_ptr<Visitor>(new BindSetterVisitor(frameBind));
    frame->acceptVisitor(vis);
}

void BindSetterVisitor::visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {
    point->setBind(bind);
}
void BindSetterVisitor::visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {
    edge->getStart()->setBind(bind);
    edge->getEnd()->setBind(bind);
}
void BindSetterVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {
    camera->getOrigin()->setBind(bind);
}


void ScaleCameraVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {
    camera->getOrigin()->acceptVisitor(p);
    auto w = camera->getWidth(), h = camera->getHeight();
    Vector3D vec(w, h, 0);
    vec = transformer->transform(vec);
    camera->setWidth(vec[0]);
    camera->setHeight(vec[1]);
}


void SelectionVisitor::visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {}
void SelectionVisitor::visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {
    auto s = norm(transformer->transform(edge->getStart()->getPoint()));
    auto e = norm(transformer->transform(edge->getEnd()->getPoint()));

    double a = e[1] - s[1];
    double b = s[0] - e[0];
    double c = -s[0]*a - s[1]*b;
    double len = (x*a + y*b + c) / sqrt(a*a+b*b);
    double val = sqrt(x*x+y*y);
    is_selected |= (fabs(len) + val)/ val < SELECTION_ERROR;
    //cout << len << ' ' << val << ' ' << (fabs(len) + val)/ val << '\n';
}
void SelectionVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {
    is_selected = false;
}


void SetColorVisitor::visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {
    point->setStyle(keeper.pointStyle);
}
void SetColorVisitor::visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {
    edge->setStyle(keeper.edgeStyle);
}
void SetColorVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {}

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p) {
    os << "<DrawablePoint:" << p.x << ' '<< p.y << ' ' << p.z << ">";
}
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e) {
    os << "<DrawableEdge>:\nfrom: " << e.x1 << ' '<< e.y1 << ' ' << e.z1 << "\nto: ";
    os << e.x2 << ' '<< e.y2 << ' ' << e.z2 <<'\n';
}

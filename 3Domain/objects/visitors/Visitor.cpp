//
// Created by kurush on 31.05.2020.
//

#include "Visitor.h"
/*
void visitPoint3D(std::shared_ptr<BaseQRPoint3D> point);
void visitEdge3D(std::shared_ptr<BaseEdge3D> edge);
void visitCamera3D(std::shared_ptr<BaseCamera3D> camera);
void visitFrame3D(std::shared_ptr<BaseFrame3D> frame);
 */

void DrawablePoint::acceptVisitor(DrawMethodVisitor &v) {v.visitDrawPoint(*this);}
void DrawableEdge::acceptVisitor(DrawMethodVisitor &v) {v.visitDrawEdge(*this);}

void DrawVisitor::visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) {
    auto p = point->getPoint();
    p = transformer->transform(p);
    p = norm(p);
    data.push_back(std::shared_ptr<DrawableData>(new DrawablePoint(p[0], p[1], p[2], point->getStyle())));
}
void DrawVisitor::visitEdge3D(std::shared_ptr<BaseEdge3D> edge) {
    auto pointer = edge->getStart();
    auto ps = norm(transformer->transform(pointer->getPoint()));
    pointer = edge->getEnd();
    auto pe = norm(transformer->transform(pointer->getPoint()));
    ps = norm(ps);
    data.push_back(std::shared_ptr<DrawableData>(new DrawableEdge(ps[0], ps[1], ps[2],
            pe[0], pe[1], pe[2], edge->getStyle())));
}
void DrawVisitor::visitCamera3D(std::shared_ptr<BaseCamera3D> camera) {}
void DrawVisitor::visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {
    for (auto obj: frame->getObjects())
        obj->acceptVisitor(this->p);
}

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p) {
    os << "<DrawablePoint:" << p.x << ' '<< p.y << ' ' << p.z << ">";
}
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e) {
    os << "<DrawableEdge>:\nfrom: " << e.x1 << ' '<< e.y1 << ' ' << e.z1 << "\nto: ";
    os << e.x2 << ' '<< e.y2 << ' ' << e.z2 <<'\n';
}

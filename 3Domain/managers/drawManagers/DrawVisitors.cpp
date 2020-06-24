//
// Created by kurush on 24.06.2020.
//

#include "DrawVisitors.h"

void DrawablePoint::acceptVisitor(sptr<QRDrawVisitor> &v) {v->visitDrawPoint(*this);}
void DrawableEdge::acceptVisitor(sptr<QRDrawVisitor> &v) {v->visitDrawEdge(*this);}

void DrawVisitor::visitPoint3D(sptr<QRPoint3D> point) {
    auto p = point->getPoint();
    p = transformer->transform(p);
    //p = norm(p);
    auto style = point->isSelected()? painter->getSelectionPointStyle() : point->getStyle();
    data.push_front(sptr<DrawableData>(new DrawablePoint(p[0], p[1], p[2], style)));
}
void DrawVisitor::visitEdge3D(sptr<QREdge3D> edge) {
    auto pointer = edge->getStart();
    auto ps = norm(transformer->transform(pointer->getPoint()));
    pointer = edge->getEnd();
    auto pe = norm(transformer->transform(pointer->getPoint()));
    auto style = edge->isSelected()? painter->getSelectionEdgeStyle() : edge->getStyle();
    data.push_back(sptr<DrawableData>(new DrawableEdge(ps[0], ps[1], ps[2],
                                                       pe[0], pe[1], pe[2], style)));
}
void DrawVisitor::visitCamera3D(sptr<QRCamera3D> camera) {}

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p) {
    os << "<DrawablePoint:" << p.x << ' '<< p.y << ' ' << p.z << ">";
}
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e) {
    os << "<DrawableEdge>:\nfrom: " << e.x1 << ' '<< e.y1 << ' ' << e.z1 << "\nto: ";
    os << e.x2 << ' '<< e.y2 << ' ' << e.z2 <<'\n';
}
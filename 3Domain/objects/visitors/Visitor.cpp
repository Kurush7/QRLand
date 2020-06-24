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


void QRVisitor::visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {
    for (auto obj: frame->getObjects())
        obj->acceptVisitor(this->p);
}

void DrawablePoint::acceptVisitor(std::shared_ptr<BaseDrawMethodVisitor> &v) {v->visitDrawPoint(*this);}
void DrawableEdge::acceptVisitor(std::shared_ptr<BaseDrawMethodVisitor> &v) {v->visitDrawEdge(*this);}

void DrawVisitor::visitPoint3D(std::shared_ptr<QRPoint3D> point) {
    auto p = point->getPoint();
    p = transformer->transform(p);
    //p = norm(p);
    auto style = point->isSelected()? painter->getSelectionPointStyle() : point->getStyle();
    data.push_front(std::shared_ptr<DrawableData>(new DrawablePoint(p[0], p[1], p[2], style)));
}
void DrawVisitor::visitEdge3D(std::shared_ptr<QREdge3D> edge) {
    auto pointer = edge->getStart();
    auto ps = norm(transformer->transform(pointer->getPoint()));
    pointer = edge->getEnd();
    auto pe = norm(transformer->transform(pointer->getPoint()));
    auto style = edge->isSelected()? painter->getSelectionEdgeStyle() : edge->getStyle();
    data.push_back(std::shared_ptr<DrawableData>(new DrawableEdge(ps[0], ps[1], ps[2],
            pe[0], pe[1], pe[2], style)));
}
void DrawVisitor::visitCamera3D(std::shared_ptr<QRCamera3D> camera) {}


void TransformVisitor::visitPoint3D(std::shared_ptr<QRPoint3D> point) {
    auto vec = point->getRelativePoint();
    vec = transformer->transform(vec);
    point->setRelativeVector(vec);
}
void TransformVisitor::visitEdge3D(std::shared_ptr<QREdge3D> edge) {}
void TransformVisitor::visitCamera3D(std::shared_ptr<QRCamera3D> camera) {
    camera->getOrigin()->acceptVisitor(p);
}
void TransformVisitor::visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {
    for (auto x: frame->getObjects())
        x->acceptVisitor(p);

}

void MoveTransformVisitor::visitPoint3D(std::shared_ptr<QRPoint3D> point) {
    auto vec = point->getRelativePoint();
    vec = transformer->transform(vec);
    point->setRelativeVector(vec);
    frameBind = point->getBind();
}

void MoveTransformVisitor::visitFrame3D(std::shared_ptr<BaseFrame3D> frame) {
    for (auto x: frame->getObjects())
        x->acceptVisitor(p);

    //set new bind for frame
    frameBind = transformer->transform(frameBind);
    auto vis = shared_ptr<QRVisitor>(new BindSetterVisitor(frameBind));
    frame->acceptVisitor(vis);
}

void BindSetterVisitor::visitPoint3D(std::shared_ptr<QRPoint3D> point) {
    point->setBind(bind);
}
void BindSetterVisitor::visitEdge3D(std::shared_ptr<QREdge3D> edge) {}
void BindSetterVisitor::visitCamera3D(std::shared_ptr<QRCamera3D> camera) {
    camera->getOrigin()->setBind(bind);
}


void ScaleCameraVisitor::visitCamera3D(std::shared_ptr<QRCamera3D> camera) {
    camera->getOrigin()->acceptVisitor(p);
    auto w = camera->getWidth(), h = camera->getHeight();
    Vector3D vec(w, h, 0);
    vec = transformer->transform(vec);
    camera->setWidth(vec[0]);
    camera->setHeight(vec[1]);
}


void SelectionVisitor::visitPoint3D(std::shared_ptr<QRPoint3D> point) {}
void SelectionVisitor::visitEdge3D(std::shared_ptr<QREdge3D> edge) {
    auto s = transformer->transform(edge->getStart()->getPoint());
    auto e = transformer->transform(edge->getEnd()->getPoint());

    double a = e[1] - s[1];
    double b = s[0] - e[0];
    double c = -s[0]*a - s[1]*b;
    double len_ab = sqrt(a*a+b*b);
    double len = (x*a + y*b + c) / len_ab;   // from full line to point: perpendicular

    double a1 = a/len_ab, b1 = b/len_ab;    // a1b1 - normal with len 1
    a1 *= len, b1 *= len;
    double x1 = x + a1, y1 = y + b1;    // coords of point projection on line

    double len2 = (x1*a + y1*b + c) / len_ab;   // distance from line. we could've chosen wrong normal
    if (fabs(len2) > QREPS)
        x1 = x - a1, y1 = y - b1;

    double dx1 = x1 - s[0], dy1 = y1 - s[1];    // vectors with edge points
    double dx2 = x1 - e[0], dy2 = y1 - e[1];

    if (dx1 * dx2 > -QREPS && dy1 * dy2 > - QREPS) {    // point is outside 'edge-rectangle'
        double len1 = sqrt(dx1*dx1  + dy1*dy1);
        len2 = sqrt(dx2*dx2  + dy2*dy2);
        len1 = min(len1,len2);
        len = sqrt(len * len + len1*len1);
    }

    double val = sqrt(x*x+y*y);
    is_selected |= (fabs(len) + val)/ val < SELECTION_ERROR;
    //cout << "edge " << s[0] << ' ' << s[1] << " <=> " << e[0] << ' ' << e[1] << " :: ";
    //cout << len << ' ' << val << ' ' << (fabs(len) + val)/ val << '\n';
}
void SelectionVisitor::visitCamera3D(std::shared_ptr<QRCamera3D> camera) {
    is_selected = false;
}


void SetColorVisitor::visitPoint3D(std::shared_ptr<QRPoint3D> point) {
    point->setStyle(keeper.pointStyle);
}
void SetColorVisitor::visitEdge3D(std::shared_ptr<QREdge3D> edge) {
    edge->setStyle(keeper.edgeStyle);
}
void SetColorVisitor::visitCamera3D(std::shared_ptr<QRCamera3D> camera) {}

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p) {
    os << "<DrawablePoint:" << p.x << ' '<< p.y << ' ' << p.z << ">";
}
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e) {
    os << "<DrawableEdge>:\nfrom: " << e.x1 << ' '<< e.y1 << ' ' << e.z1 << "\nto: ";
    os << e.x2 << ' '<< e.y2 << ' ' << e.z2 <<'\n';
}

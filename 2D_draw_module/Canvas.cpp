//
// Created by kurush on 13.02.2020.
//

#include "Canvas.h"
#include <cmath>
#include <algorithm>

using namespace std;


CanvasPanel::CanvasPanel(int h, int w): h(h), w(w) {
    setFixedHeight(1.5*h);
    setFixedWidth(0);
    layout = new QHBoxLayout();
    setLayout(layout);
    layout->setAlignment(Qt::AlignRight|Qt::AlignBottom);
}
void CanvasPanel::addButton(QPushButton* btn, function<void()> action) {
    setFixedWidth(width() + 1.2*w);
    btn->setFixedWidth(w);
    btn->setFixedHeight(h);
    layout->addWidget(btn);
    connect(btn, &QPushButton::clicked, action);
}


void CanvasViewer::checkD() {
    dx = max(0., dx);
    dy = max(0., dy);
    dx = min(dx, src->width() * (1 - 1/k));
    dy = min(dy, src->height() * (1 - 1/k));
}
void CanvasViewer::checkK() {
    k = max(1., k);
}

Borders CanvasViewer::getBorders() {
    return Borders(dx, dx + src->width()/k, src->height() - dy, src->height() - dy - src->height()/k);
}

void CanvasViewer::zoom(Point center, double k_new) {
    double dx_new = (center.x - src->width()/2) / k / zoomSensivity;
    double dy_new = (center.y - src->height()/2) / k / zoomSensivity;
    k *= k_new;
    checkK();

    dx += dx_new;
    dy += dy_new;
    checkD();
}

void CanvasViewer::move(double _dx, double _dy) {
    dx += _dx / moveSensivity;
    dy += _dy / moveSensivity;
    checkD();
}


void CanvasViewer::reset() {
    dx = dy = 0;
    k = 1;
}

int CanvasViewer::getView(QImage *img) {
    for (int i = 0; i < img->height(); ++i)
        for (int j = 0; j < img->width(); ++j) {
            img->setPixel(j, i, src->pixel(dx + j/k, dy + i/k));
        }
    return 0;
}


Canvas::~Canvas() {
    delete timer;
    if (from)
        delete from;
    if (trans)
        delete trans;
    if (ownData)
        for (auto obj: objects)
            delete obj;
    delete img;
    delete zoomed_img;
}

Canvas::Canvas(int w, int h, QWidget *parent, QColor fill)
        : QWidget(parent), fill(fill), ownData(ownData)
{
    setFixedSize(w, h);

    QPalette p(palette());
    p.setColor(QPalette::Background, QColor(255,255,255,255));
    setAutoFillBackground(true);
    setPalette(p);

    coordHint = new QLabel("aaa", this);
    coordHint->setStyleSheet("QLabel {"
                         "border-style: solid;"
                         "border-width: 1px;"
                         "border-color: black; "
                         "}");
    coordHint->setVisible(false);
    timer = new QTimer();

    Borders to(0, width(), 0, height());
    trans = new Transformer(to);

    connect(timer, SIGNAL(timeout()), this, SLOT(HintTimerAlarm()));

    img = new QImage(w, h, QImage::Format_ARGB32);
    dyn_img = new QImage(w, h, QImage::Format_ARGB32);
    zoomed_img = new QImage(w, h, QImage::Format_ARGB32);
    viewer = CanvasViewer(img);
    dyn_viewer = CanvasViewer(dyn_img);

    repaint();
}

void Canvas::refillBg() {
    fillBgOnly = true;
    repaint();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(img);
    QPainter painterMain(this);

    if (updateImageOnlyFlag) {
        viewer.getView(zoomed_img);  // copy from img to zoomed_img (with zooming)
        painterMain.drawImage(QRect(0,0,width(), height()), *zoomed_img);
        return;
    }

    if (clearOnUpdate || fillBgOnly) {
        painter.eraseRect(0, 0, width(), height());
        painter.fillRect(0, 0, width(), height(), QBrush(fill));
        if (fillBgOnly) {
            fillBgOnly = false;
            viewer.getView(zoomed_img);  // copy from img to zoomed_img (with zooming)
            painterMain.drawImage(QRect(0,0,width(), height()), *zoomed_img);
            return;
        }
    }

    if (zoomOnly) {
        painterMain.drawImage(QRect(0,0,width(), height()), *zoomed_img);
        zoomOnly = false;
        return;
    }

    if (scaleFit)
        ScaleObjects();

    if (showAxis && !objects.empty()) {
        Borders to(0, width(), 0, height());
        if (!from) {
            auto x = *(objects.begin());
            auto fr = x->getBorders();;
            for (auto obj: objects)
                fr.keepMax(obj->getBorders());
            from = new Borders(fr);
        }

        auto pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        Point tr = trans->fit(Point(0, 0));
        painter.drawLine(tr.x, to.y_min, tr.x, to.y_max);
        painter.drawLine(tr.x - 5, to.y_min + 10, tr.x, to.y_min);
        painter.drawLine(tr.x + 5, to.y_min + 10, tr.x, to.y_min);
        painter.drawText(tr.x + 15, to.y_min + 15, "Y");

        painter.drawLine(to.x_min, tr.y, to.x_max, tr.y);
        painter.drawLine(to.x_max - 10, tr.y - 5, to.y_max, tr.y);
        painter.drawLine(to.x_max - 10, tr.y + 5, to.y_max, tr.y);
        painter.drawText(to.x_max - 15, tr.y + 25, "X");

    }

    if (!trans) {
        painterMain.eraseRect(0, 0, width(), height());
        painterMain.fillRect(0, 0, width(), height(), QBrush(fill));
        return;
    }

    for (auto obj: objects) {
        obj->draw(painter, trans, true);
    }

    viewer.getView(zoomed_img);  // copy from img to zoomed_img (with zooming)
    painterMain.drawImage(QRect(0,0,width(), height()), *zoomed_img);

    QPainter dyn_painter(dyn_img);
    dyn_img->fill(QColor(0,0,0,0));
    for (auto obj: dynamicObjects) {
        obj->draw(dyn_painter, trans, true);
    }
    dyn_viewer.getView(zoomed_img);  // copy from img to zoomed_img (with zooming)
    painterMain.drawImage(QRect(0,0,width(), height()), *zoomed_img);
}


void Canvas::ScaleObjects(double keep) {
    if (objects.empty())
        return;
    if (trans) delete trans;
    Borders to(0, width(), 0, height()), fr;
    if (from)
        delete from;

    auto x = *(objects.begin());
    fr = x->getBorders();
    for (auto obj: objects)
        fr.keepMax(obj->getBorders());
    from = new Borders(fr);

    trans = new Scaler(*from, to, keep);
}

void Canvas::enablePanel() {
    panel = new CanvasPanel(32, 64);
    panel->addButton(new QPushButton("clear", this), [this](){this->clearObjects();});
    panel->setGeometry(this->width() - panel->width(), this->height()-panel->height(),
                       panel->width(), panel->height());
    panel->setParent(this);
}

void Canvas::setBgColor(QColor color) {
    fill = color;
}
void Canvas::setFromBorders(Borders fr) {
    from = new Borders(fr);
    ScaleObjects(0);
}

void Canvas::addObject(DrawableObject *obj, bool dynamic) {
    if (!dynamic)
        objects.push_back(obj);
    else
        dynamicObjects.push_back(obj);
}
void Canvas::clearObjects(bool dynamic) {
    vector<DrawableObject*>& objs = dynamic? dynamicObjects: objects;
    if (ownData)
        for (auto obj: objs)
            delete obj;
    objects.clear();
}
void Canvas::deleteObject(DrawableObject *obj, bool dynamic)
{
    // todo own data?
    vector<DrawableObject*>& objs = dynamic? dynamicObjects: objects;
    for (auto it = objs.begin(); it != objs.end(); ++it) {
        if (*it == obj) {
            objs.erase(it);
            return;
        }
    }
}
void Canvas::setObjects(std::vector<DrawableObject*> objs, bool dynamic)
{
    vector<DrawableObject*>& o = dynamic? dynamicObjects: objects;
    o = objs;
}
void Canvas::setObjects(std::set<DrawableObject*> objs, bool dynamic) {
    vector<DrawableObject*>& o = dynamic? dynamicObjects: objects;
    clearObjects(dynamic);
    for (auto a: o)
        objects.push_back(a);
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        viewer.reset();
        dyn_viewer.reset();
        update();
        return;
    }
    if (event->button() == Qt::LeftButton)
        mousePressedPnt = Point(event->pos().x(), event->pos().y());

    float x = event->pos().x();
    float y = event->pos().y();
    coordHint->setGeometry(x, y, coordHint->width(), coordHint->height());

    Point p(x,y);
    if (trans) {
        p = trans->fitBack(p);
        Point min = trans->fitBack(Point(0,0));
        Point max = trans->fitBack(Point(width(), height()));
        swap(min.y, max.y);
        double kx = (p.x - min.x) / (max.x - min.x);
        double ky = (p.y - min.y) / (max.y - min.y);

        auto bords = viewer.getBorders();
        p.x = bords.x_min + kx * (bords.x_max - bords.x_min);
        p.y = bords.y_min + ky * (bords.y_max - bords.y_min);
    }

    x = p.x;
    y = p.y;
    x = floor( x*100 )/100;
    y = floor( y*100 )/100;

    coordHint->setText("(" + QString::number(x) + "; " + QString::number(y) + ")");
    coordHint->setVisible(true);

    timer->start(3000); // И запустим таймер
}
void Canvas::HintTimerAlarm()
{
    coordHint->setVisible(false);
    timer->stop();
}

void Canvas::wheelEvent(QWheelEvent *event) {
    double x = event->pos().x();
    double y = event->pos().y();
    Point p(x,y);

    if (event->delta() > 0)
        viewer.zoom(p, zoomRatio),
        dyn_viewer.zoom(p, zoomRatio);
    else
        viewer.zoom(p, 1 / zoomRatio),
        dyn_viewer.zoom(p, 1 / zoomRatio);

    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        double dx = event->pos().x() - mousePressedPnt.x;
        double dy = event->pos().y() - mousePressedPnt.y;
        viewer.move(dx, -dy);
        dyn_viewer.move(dx, -dy);
        update();
    }
}

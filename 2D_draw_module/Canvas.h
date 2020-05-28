//
// Created by kurush on 13.02.2020.
//

#ifndef KG_GRAPHICFIELD_H
#define KG_GRAPHICFIELD_H

#include <QGraphicsView>
#include <iostream>
#include <set>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>


#include <memory>
#include <iostream>
#include <functional>


#include "figures/Primitives.h"
#include "figures/AlgoedLine.h"

const double zoomRatio = 1.1;
const double zoomSensivity = 4;
const double moveSensivity = 32;

class CanvasPanel: public QWidget {
Q_OBJECT
public:
    CanvasPanel(int h=40, int w=64);
    void addButton(QPushButton*, std::function<void()>);
private:
    int h, w;
    QHBoxLayout *layout;
};

class CanvasViewer {
public:
    CanvasViewer(QImage *img = nullptr): src(img), k(1), dx(0), dy(0) {}
    int getView(QImage*);
    Borders getBorders();

    void zoom(Point, double);
    void move(double, double);

    void reset();
private:
    void checkD();
    void checkK();

    QImage *src;
    double dx, dy, k;
};


// todo PolyBuilder may not work correctly with memory when flag ownData is true...
class Canvas : public QWidget {
Q_OBJECT
public:
    explicit Canvas(int w, int h, QWidget *parent = nullptr,
                    QColor fill = Qt::white);
    ~Canvas() override;

    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);  // todo keeps original press-point even when timely working

    void ScaleObjects(double = 0.1);

    void setUpdateImageOnly(bool f) {
        updateImageOnlyFlag = f;
    }

    virtual void enablePanel();
    void setOwnData(bool f) {ownData = f;}
    void setAutoScaleFit(bool f) {scaleFit = f;}
    void setShowAxis(bool f) {showAxis = f;}
    void setBgColor(QColor);
    void setFromBorders(Borders);

    void setClearOnUpdate(bool c) {clearOnUpdate = c;}
    void refillBg();

    Transformer* getTransformer() {return trans;}

    virtual void addObject(DrawableObject *obj, bool dynamic=false);
    void setObjects(std::set<DrawableObject*> objs, bool dynamic=false);    // todo don't use if want correct allignes!
    void setObjects(std::vector<DrawableObject*> objs, bool dynamic=false);    // todo don't use if want correct allignes!
    virtual void clearObjects(bool dynamic=false);
    virtual void deleteObject(DrawableObject *obj, bool dynamic=false);
    std::vector<DrawableObject*>& getObjects() {return objects;}

    QImage* getImage() {return img;}

protected:
    bool updateImageOnlyFlag = false;
    CanvasPanel *panel = nullptr;
    QLabel *coordHint;
    QTimer *timer;
    QColor fill;

    std::vector<DrawableObject*> objects;
    std::vector<DrawableObject*> dynamicObjects;
    Transformer *trans;
    CanvasViewer viewer, dyn_viewer;

    Borders *from = nullptr;
    Point mousePressedPnt;
    QImage *img, *dyn_img, *zoomed_img;
    bool scaleFit = true, ownData = false, showAxis = false;
    bool clearOnUpdate = true;
    bool fillBgOnly = false;
    bool zoomOnly = false;
private slots:
    void HintTimerAlarm();
};

#endif //KG_GRAPHICFIELD_H
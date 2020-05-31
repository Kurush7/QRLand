#ifndef LAB_10_TASKMANAGER_H
#define LAB_10_TASKMANAGER_H

#include <QObject>
#include <fstream>
#include <QMessageBox>

#include "Canvas3DViewer.h"
#include "../3Domain/Lab3Facade.h"


class QTPainter: public Painter {
public:
    virtual void drawPoint(double x, double y, QRPointStyle s) {
        canvas->addObject(new Point(x, y, defineColor(s.color)));
    }
    virtual void drawEdge(double x1, double y1, double x2, double y2, QREdgeStyle s) {
        canvas->addObject(new Line(x1, y1, x2, y2, defineColor(s.color)));
    }

private:
    QColor defineColor(QRColor c) {
        QColor st;
        if (c == black) st = black;
        if (c == white) st = white;
        if (c == green) st = green;
        if (c == blue) st = blue;
        if (c == yellow) st = yellow;
        if (c == red) st = red;
        return st;
    }

    Canvas3DViewer *canvas;
};

class MainWindow;
class Presenter: public QObject {
Q_OBJECT
public:
    explicit Presenter(MainWindow &w): window(w) {}

    void backChangeColor();

    void loadModel();
    void openFile();

    void select(double x, double y);
    void selBind();
    void selChangeColor();
    void selDelete();

    void transform(TransformStateDir);
private:
    MainWindow &window;
};


#endif //LAB_10_TASKMANAGER_H

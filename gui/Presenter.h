#ifndef LAB_10_TASKMANAGER_H
#define LAB_10_TASKMANAGER_H

#include <QObject>
#include <fstream>
#include <QMessageBox>

#include "Canvas3DViewer.h"
#include "../3Domain/Lab3Facade.h"

// todo problem in selection: multiple zones

const double MOVE_UNIT_VAL = 1;
const double SCALE_UNIT_VAL = 1.05;
const double ROTATE_UNIT_VAL = 0.05;

QColor defineColor(QRColor c);

class QTPainter: public Painter {
public:
    QTPainter(Canvas3DViewer *c): canvas(c) {}
    virtual void drawPoint(double x, double y, QRPointStyle s) {
        points.push_back(new Point(x, y, defineColor(s.color)));
        //canvas->addObject(new Point(x, y, defineColor(s.color)));
    }
    virtual void drawEdge(double x1, double y1, double x2, double y2, QREdgeStyle s) {
        lines.push_back(new Line(x1, y1, x2, y2, defineColor(s.color)));
    }

    virtual void setFrame(double x_center, double y_center, double w, double h) {
        canvas->setFromBorders(Borders(x_center-w/2, x_center+w/2, y_center-h/2,  y_center+h/2));
    }

    virtual void paint() {
        canvas->clearObjects();
        // todo cutting

        for (auto p: points)
            canvas->addObject(p);
        for (auto l: lines)
            canvas->addObject(l);

        canvas->repaint();
    }
    virtual void reset() {
        points.clear();
        lines.clear();
    }

    virtual QRPointStyle getSelectionPointStyle() {return QRPointStyle(QRColor("white"));}
    virtual QREdgeStyle getSelectionEdgeStyle() {return QREdgeStyle(QRColor("red"));}

private:
    Canvas3DViewer *canvas;
    vector<Line*> lines;
    vector<Point*> points;
};

class MainWindow;
class Presenter: public QObject {
Q_OBJECT
public:
    explicit Presenter(MainWindow &w);
    void setPainter();

    void backChangeColor();

    void loadModel();
    void openFile();
    void undo();

    void select(double x, double y);
    void selChangeColor();
    void selDelete();

    void transform(TransformStateDir);

private:
    void defineTransformParams(double&, double&, double&, TransformStateDir);

    MainWindow &window;

    std::shared_ptr<Painter> painter = nullptr;
    Lab3Facade facade;
};


#endif //LAB_10_TASKMANAGER_H

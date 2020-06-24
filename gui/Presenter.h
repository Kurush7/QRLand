#ifndef LAB_10_TASKMANAGER_H
#define LAB_10_TASKMANAGER_H

#include <QObject>
#include <fstream>
#include <QMessageBox>

#include "Canvas3DViewer.h"
#include "../3Domain/Lab3Facade.h"

#include "QTPainter.h"

const double MOVE_UNIT_VAL = 1;
const double SCALE_UNIT_VAL = 1.05;
const double ROTATE_UNIT_VAL = 0.05;


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

    sptr<BasePainterCreator> creator;
    sptr<Lab3Facade> facade;
};


#endif //LAB_10_TASKMANAGER_H

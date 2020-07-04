#ifndef LAB_10_TASKMANAGER_H
#define LAB_10_TASKMANAGER_H

#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>

#include "2Domain.h"
#include "Facade.h"


const double MOVE_UNIT_VAL = 1;
const double SCALE_UNIT_VAL = 1.05;
const double ROTATE_UNIT_VAL = 0.05;

const int REPAINT_TIME = 100;

class MainWindow;
class Presenter: public QObject {
Q_OBJECT
public:
    explicit Presenter(MainWindow &w);
    void undo();
    void transform(QRKey);
    void addCube(double a);

private:
    void defineTransformParams(double&, double&, double&, QRKey);
    void draw(bool reset=false);
    double draw_time_msec = 0;
    size_t draw_cnt = 0;

    MainWindow &window;

    sptr<QRImage> image;
    sptr<Facade> facade;
};


#endif //LAB_10_TASKMANAGER_H

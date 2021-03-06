#ifndef LAB_10_TASKMANAGER_H
#define LAB_10_TASKMANAGER_H

#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>

#include "2Domain.h"
#include "Facade.h"


const float MOVE_UNIT_VAL = 0.3;
const float SCALE_UNIT_VAL = 1.05;
const float SCALE_FULL_ROTATE_VAL = 1.5;
const float ROTATE_UNIT_VAL = 0.05;

const int REPAINT_TIME = 100;

class MainWindow;
class Presenter: public QObject {
Q_OBJECT
public:
    explicit Presenter(MainWindow &w);
    void undo();
    void transform(QRKey);
    void scale(float times);
    void addCube(float a);

private:
    void defineTransformParams(float&, float&, float&, QRKey);
    void draw(bool reset=false);
    float draw_time_msec = 0;
    size_t draw_cnt = 0;

    MainWindow &window;

    sptr<QRImage> image;
    sptr<Facade> facade;
};


#endif //LAB_10_TASKMANAGER_H

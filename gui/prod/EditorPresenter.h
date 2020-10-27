#ifndef EDITOR_PRESENTER_H
#define EDITOR_PRESENTER_H

#include <QObject>
#include <fstream>
#include <QMessageBox>
#include <chrono>

#include "2Domain.h"
#include "Facade.h"


const float MOVE_UNIT_VAL = 1;
const float SCALE_UNIT_VAL = 1.05;
const float SCALE_FULL_ROTATE_VAL = 1.5;
const float ROTATE_UNIT_VAL = 0.05;

const int REPAINT_TIME = 100;

class EditorWindow;
class EditorPresenter: public QObject {
Q_OBJECT
public:
    explicit EditorPresenter(EditorWindow &w, ModelInitData dt, bool initFacade = true);
    ~EditorPresenter() {
        facade.reset();
    }
    void undo();
    void transform(QRKey);
    void transformMouse(float dx, float dy);
    void scale(float times);
    void changeCamera();
    void setWaterVisible();
    void setShadesVisible();
    void erosionIteration();
    void scaleGrid();
    void process();
    void save() {}
    void updateMiniCoords(float x, float y);

    sptr<Facade> facade;
private:
    void defineTransformParams(float&, float&, float&, QRKey);
    void draw(bool reset=false);

    float draw_time_msec = 0;
    size_t draw_cnt = 0;

    EditorWindow &window;

    sptr<QRImage> image, hmap_image;

    float rotCamera = 0;

    bool is1Active=true;
};


#endif //EDITOR_PRESENTER_H

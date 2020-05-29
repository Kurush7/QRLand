#ifndef LAB_10_TASKMANAGER_H
#define LAB_10_TASKMANAGER_H

#include <QObject>
#include <QThread>
#include <fstream>
#include <QMessageBox>

#include "common/ThreadFunctionCall.h"
#include "figures/AlgoedCircle.h"
#include "figures/AlgoedEllipse.h"
#include "figures/Function3D.h"
#include "Canvas3DViewer.h"

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

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

class Lab10_View;
class Controller: public QObject {
Q_OBJECT
public:
    explicit Controller(Lab10_View &);
private:
    Lab10_View &window;
};


#endif //LAB_10_TASKMANAGER_H

#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <cstdlib>
#include <cstdio>

#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "Presenter.h"
#include "../../globalDefines.h"
#include "2Domain.h"

const int erosionFPS = 10;

class MainWindow: public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
private:
    friend class Presenter;
    sptr<Presenter> presenter;

    void decorate();
    void addLogic();
    void setDarkTheme();

    QWidget *mainWidget;
    QRLayoutManager *ui;
    sptr<QRCanvas> canvas, hmap;

    QRadioButton *moveRad, *rotateRad, *scaleRad;
    QRadioButton *view1Rad, *view2Rad;
    QButtonGroup *g1, *g2;

    QRLabel *settingsLabel, *drawTimeLabel, *cameraLabel, *erosionLabel;

    QCheckBox *waterCheckBox;

    QPushButton *undoBtn, *erosionStart, *erosionEnd;

    QTimer erosionTimer;
};

#endif //MAIN_WINDOW

#ifndef EDITOR_WINDOW
#define EDITOR_WINDOW

#include <cstdlib>
#include <cstdio>

#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "EditorPresenter.h"
#include "../../globalDefines.h"
#include "2Domain.h"

const int erosionFPS = 30;

class EditorWindow: public QMainWindow {
Q_OBJECT
public:
    explicit EditorWindow(QWidget *parent = nullptr);
    virtual ~EditorWindow();
private:
    friend class EditorPresenter;
    sptr<EditorPresenter> presenter;

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

    QCheckBox *waterCheckBox, *shadesCheckBox;

    QPushButton *undoBtn, *erosionStart, *erosionEnd, *scaleGrid, *process;

    QTimer erosionTimer;
};

#endif //EDITOR_WINDOW

#ifndef EDITOR_WINDOW
#define EDITOR_WINDOW

#include <cstdlib>
#include <cstdio>

#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "EditorPresenter.h"
#include "../../globalDefines.h"
#include "2Domain.h"
#include "EditorWaterWidget.h"
#include "EditorHMapWidget.h"
#include "EditorMiscWidget.h"
#include "views/QRMenu.h"

class EditorWindow: public QMainWindow {
Q_OBJECT
public:
    explicit EditorWindow(ModelInitData dt = ModelInitData(), std::string initFacade = "",
            QWidget *parent = nullptr);
    virtual ~EditorWindow();

    sptr<EditorPresenter> presenter;
private:
    friend class EditorPresenter;

    void decorate();
    void addLogic();
    void setDarkTheme();

    QWidget *mainWidget;
    QRLayoutManager *ui;
    sptr<QRCanvas> canvas, canvasMini;

    QRLabel *drawTimeLabel;

    // mini-screen data
    QCheckBox *miniScreenHmap, *miniScreenWater, *miniScreenPlates;
    QRLabel *miniScreenX, *miniScreenY, *miniScreenZ;

    // big data
    QRadioField *multiRadio;
    EditorWaterWidget *waterWidget;
    EditorHMapWidget *hmapWidget;
    EditorMiscWidget *miscWidget;

    QTimer erosionTimer;
};

#endif //EDITOR_WINDOW

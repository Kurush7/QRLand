#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <cstdlib>
#include <cstdio>

#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "Presenter.h"
#include "../../globalDefines.h"
#include "2Domain.h"

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
    sptr<QRCanvas> canvas;

    QRadioButton *moveRad, *rotateRad, *scaleRad;

    QRLabel *settingsLabel;
    QRLabel *figureColorLabel;
    ColorEdit *figureColorEdit;

    QRLabel *drawTimeLabel;
    QRLabel *addLabel;
    QRLabel *xDropLabel, *yDropLabel, *zDropLabel;
    QLineEdit *xDropEdit, *yDropEdit, *zDropEdit;

    QPushButton *addBtn, *undoBtn;
};

#endif //MAIN_WINDOW

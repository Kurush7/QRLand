#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <cstdlib>
#include <cstdio>

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QMainWindow>
#include <iostream>
#include <QFile>
#include <QPalette>
#include <QtWidgets/QVBoxLayout>
#include <QFileDialog>
#include <QFormLayout>
#include <QColorDialog>
#include <QComboBox>
#include <QSplitter>
#include <QDebug>
#include <QCheckBox>

#include "widgets/ColorEdit.h"
#include "widgets/LineSpacer.h"
#include "widgets/QRLabel.h"
#include "widgets/CustomDoubleValidator.h"
#include "widgets/QRadioField.h"
#include "widgets/MultiRadioField.h"

#include "common/QRLayoutManager.h"
#include "common/Themes.h"
#include "Canvas3DViewer.h"
#include "figures/DropPolyFiller.h"
#include "figures/PolygonCutter.h"
#include "figures/AlgoedLine.h"
#include "figures/Function3D.h"
#include "Controller.h"


class Lab10_View: public QMainWindow {
Q_OBJECT
public:
    explicit Lab10_View(QWidget *parent = nullptr);
    virtual ~Lab10_View();
private:
    friend class Controller;
    void decorate();
    void addLogic();
    void setDarkTheme();

    QWidget *mainWidget;
    QRLayoutManager *ui;
    Canvas3DViewer *canvas;
    QRMultiRadioField *visibilityManager;


    QRLabel *settingsLabel, *selectionLabel;
    QRLabel *backColorLabel, *selColorLabel;
    ColorEdit *backColorEdit, *selColorEdit;

    QRLabel *loadLabel, *fileNameLabel;
    QRLabel *xDropLabel, *yDropLabel, *zDropLabel;
    QLineEdit *xDropEdit, *yDropEdit, *zDropEdit;
    QPushButton *modelLoadBtn, *openFileBtn;
    QLineEdit *fileName;

    QPushButton *selBindBtn, *selDeleteBtn;

private slots:
};

#endif //MAIN_WINDOW

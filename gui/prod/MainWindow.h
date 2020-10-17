#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <cstdlib>
#include <cstdio>

#include "../QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "../views/QRViews.h"
#include "../../globalDefines.h"

#include "EditorWindow.h"

#include "2Domain.h"

class MainWindow: public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
private:

    void decorate();
    void addLogic();
    void setDarkTheme();

    QWidget *mainWidget;
    QRLayoutManager *ui;

    QPushButton *newBtn, *openBtn;
    QRMenu *menu;
    QFileDialog *openFile;

    EditorWindow *editorWindow;
};

#endif //MAIN_WINDOW

#include "MainWindow.h"

using namespace std;

void MainWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

MainWindow::~MainWindow() {
    delete visibilityManager;
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setDarkTheme();

    canvas = new Canvas3DViewer(500, 500, this,
                        QColor(40, 40, 40));
    canvas->setOwnData(true);
    canvas->setAutoScaleFit(false);
    canvas->refillBg();
    //canvas->setAutoScaleFit(true);
    presenter = sptr<Presenter>(new Presenter(*this));


    visibilityManager = new QRMultiRadioField("selection", 0);


    moveRad = new QRadioButton("перемещение", this);
    rotateRad = new QRadioButton("вращение", this);
    scaleRad = new QRadioButton("масштабирование", this);

    loadLabel = new QRLabel("новая модель", 150, this);
    selectionLabel = new QRLabel("изменить", 150, this);
    settingsLabel = new QRLabel("настройки", 150, this);

    backColorLabel = new QRLabel("цвет фона", 100, this);
    selPointColorLabel = new QRLabel("цвет точек", 100,this);
    selEdgeColorLabel = new QRLabel("цвет ребер", 100,this);
    fileNameLabel = new QRLabel("файл", 100, this);
    /*xDropLabel = new QRLabel("x", 10,this);
    yDropLabel = new QRLabel("y", 10,this);
    zDropLabel = new QRLabel("z", 10,this);

    xDropEdit = new QLineEdit();
    yDropEdit = new QLineEdit();
    zDropEdit = new QLineEdit();*/
    fileName = new QLineEdit();

    backColorEdit = new ColorEdit();
    selPointColorEdit = new ColorEdit();
    selEdgeColorEdit = new ColorEdit();

    modelLoadBtn = new QPushButton("добавить модель", this);
    //selBindBtn = new QPushButton("привязать камеру", this);
    selDeleteBtn = new QPushButton("удалить", this);
    openFileBtn = new QPushButton("открыть...", this);
    undoButton = new QPushButton("откатить", this);


    ui = new QRLayoutManager("global", QRHor);
    ui->addLayers("left right", QRVert);
    ui->addWidgets({{"canvas", canvas}}, "left");

    ui->goToPath("right");
    ui->addLayers("settings $ load $ selection", QRVert);

    ui->addLayers("label color move scale rotate undo", QRHor, "settings");
    ui->addWidgets({{"label", settingsLabel}}, "settings/label");
    ui->addWidgets({{"radio", moveRad}}, "settings/move");
    ui->addWidgets({{"radio", scaleRad}}, "settings/scale");
    ui->addWidgets({{"radio", rotateRad}}, "settings/rotate");
    ui->addWidgets({{"label", backColorLabel}, {"color", backColorEdit}}, "settings/color");
    ui->addWidgets({{"undo", undoButton}}, "settings/undo");

    ui->addLayers("label file button", QRHor, "load");
    ui->addWidgets({{"label", loadLabel}}, "load/label");
    ui->addWidgets({{"label", fileNameLabel}, {"edit", fileName},
                    {"open", openFileBtn}}, "load/file");
    //ui->addWidgets({{"xlabel", xDropLabel}, {"xedit", xDropEdit},
    //                {"ylabel", yDropLabel}, {"yedit", yDropEdit},
    //                {"zlabel", zDropLabel}, {"zedit", zDropEdit}}, "load/coords");
    ui->addWidgets({{"btn", modelLoadBtn}}, "load/button");

    ui->addLayers("label pcolor ecolor delete", QRHor, "selection");
    ui->addWidgets({{"label", selectionLabel}}, "selection/label");
    //ui->addWidgets({{"bind", selBindBtn}}, "selection/bind");
    ui->addWidgets({{"label", selPointColorLabel}, {"color", selPointColorEdit}}, "selection/pcolor");
    ui->addWidgets({{"label", selEdgeColorLabel}, {"color", selEdgeColorEdit}}, "selection/ecolor");
    ui->addWidgets({{"delete", selDeleteBtn}}, "selection/delete");

    mainWidget = new QWidget();
    mainWidget->setLayout(ui->getRootLayout());
    setCentralWidget(mainWidget);

    decorate();
    addLogic();
}

void MainWindow::decorate() {
    visibilityManager->setActive("selection", false);

    auto sepLays = ui->getSepLayers();
    for (auto lay = sepLays.begin(); lay != sepLays.end(); ++lay) {
        (*lay)->addChild(new QRLayoutNode("sep", QRHor, nullptr, new LineSpacer("color:rgb(100,100,100)", 300)));
        (*lay)->getLayout()->setAlignment(Qt::AlignCenter);
    }

    moveRad->setChecked(true);

    modelLoadBtn->setFixedWidth(140);
    selDeleteBtn->setFixedWidth(80);
    undoButton->setFixedWidth(80);
    //selBindBtn->setFixedWidth(140);
    fileName->setFixedWidth(200);
    fileNameLabel->setFixedWidth(40);

    //xDropEdit->setFixedWidth(64);
    //yDropEdit->setFixedWidth(64);
    //zDropEdit->setFixedWidth(64);
    backColorEdit->setFixedWidth(64);
    selPointColorEdit->setFixedWidth(64);
    selEdgeColorEdit->setFixedWidth(64);

    loadLabel->setObjectName("headerLabel");
    settingsLabel->setObjectName("headerLabel");
    selectionLabel->setObjectName("headerLabel");

    backColorEdit->setColor(QColor(40, 40, 40));
    selPointColorEdit->setColor(QColor(40, 40, 160));
    selEdgeColorEdit->setColor(QColor(40, 160, 40));

    visibilityManager->addWidgets(ui->getAllWidgets("right/selection", true), "selection");
    //visibilityManager->changeActive("selection");
}

void MainWindow::addLogic() {
    connect(canvas, &Canvas3DViewer::sendModify,[this](TransformStateDir d){presenter->transform(d);});

    connect(backColorEdit, &QPushButton::clicked, [this]() {presenter->backChangeColor();});

    connect(openFileBtn, &QPushButton::clicked, [this]() {presenter->openFile();});
    connect(modelLoadBtn, &QPushButton::clicked, [this]() {presenter->loadModel();});

    connect(canvas, &Canvas3DViewer::mousePressed, [this](float x, float y) {presenter->select(x, y);});
    //connect(selBindBtn, &QPushButton::clicked, [this]() {presenter->selBind();});
    connect(selPointColorEdit, &QPushButton::clicked, [this]() {presenter->selChangeColor();});
    connect(selEdgeColorEdit, &QPushButton::clicked, [this]() {presenter->selChangeColor();});
    connect(selDeleteBtn, &QPushButton::clicked, [this]() {presenter->selDelete();});
    connect(undoButton, &QPushButton::clicked, [this]() {presenter->undo();});
}


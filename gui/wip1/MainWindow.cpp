#include "MainWindow.h"

using namespace std;

void MainWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

MainWindow::~MainWindow() {}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setDarkTheme();

    canvas = sptr<QRCanvas> (new QRCanvas(512, 512, this));

    moveRad = new QRadioButton("move", this);
    rotateRad = new QRadioButton("rotate", this);
    scaleRad = new QRadioButton("scale", this);

    settingsLabel = new QRLabel("настройки", 120, this);

    addLabel = new QRLabel("добавление модели:", 140, this);
    figureColorLabel = new QRLabel("цвет модели", 100,this);
    xDropLabel = new QRLabel("x", 10,this);
    yDropLabel = new QRLabel("y", 10,this);
    zDropLabel = new QRLabel("z", 10,this);
    drawTimeLabel = new QRLabel("время отрисовки:", 500,this);

    xDropEdit = new QLineEdit();
    yDropEdit = new QLineEdit();
    zDropEdit = new QLineEdit();
    figureColorEdit = new ColorEdit();

    addBtn = new QPushButton("добавить модель", this);
    undoBtn = new QPushButton("откатить", this);

    ui = new QRLayoutManager("global", QRHor);
    ui->addLayers("left right", QRVert);
    ui->addWidgets({{"canvas", canvas.get()}, {"label", drawTimeLabel}}, "left");

    ui->goToPath("right");
    ui->addLayers("settings $ add", QRVert);

    ui->addLayers("label move scale rotate undo", QRHor, "settings");
    ui->addWidgets({{"radio", moveRad}}, "settings/move");
    ui->addWidgets({{"radio", scaleRad}}, "settings/scale");
    ui->addWidgets({{"radio", rotateRad}}, "settings/rotate");
    ui->addWidgets({{"label", settingsLabel}}, "settings/label");
    ui->addWidgets({{"undo", undoBtn}}, "settings/undo");

    ui->addLayers("label add color coords button", QRHor, "add");
    ui->addWidgets({{"label", addLabel}}, "add/label");
    ui->addWidgets({{"label", figureColorLabel}, {"color", figureColorEdit}}, "add/color");
    ui->addWidgets({{"xlabel", xDropLabel}, {"xedit", xDropEdit},
                   {"ylabel", yDropLabel}, {"yedit", yDropEdit},
                   {"zlabel", zDropLabel}, {"zedit", zDropEdit}}, "add/coords");
    ui->addWidgets({{"btn", addBtn}}, "add/button");

    mainWidget = new QWidget();
    mainWidget->setLayout(ui->getRootLayout());
    setCentralWidget(mainWidget);

    decorate();
    addLogic();
    presenter = sptr<Presenter>(new Presenter(*this));
}

void MainWindow::decorate() {
    auto sepLays = ui->getSepLayers();
    for (auto lay = sepLays.begin(); lay != sepLays.end(); ++lay) {
        (*lay)->addChild(new QRLayoutNode("sep", QRHor, nullptr, new LineSpacer("color:rgb(100,100,100)", 300)));
        (*lay)->getLayout()->setAlignment(Qt::AlignCenter);
    }

    rotateRad->setChecked(true);

    addBtn->setFixedWidth(140);
    xDropEdit->setFixedWidth(64);
    yDropEdit->setFixedWidth(64);
    zDropEdit->setFixedWidth(64);
    figureColorEdit->setFixedWidth(64);

    undoBtn->setFixedWidth(64);

    addLabel->setObjectName("headerLabel");
    settingsLabel->setObjectName("headerLabel");

    figureColorEdit->setColor(QColor(40, 40, 160));
}

void MainWindow::addLogic() {
    connect(canvas.get(), &QRCanvas::QRKeyPressed,
            [this](QRKey k, QRModifiers m) {presenter->transform(k);});
    connect(canvas.get(), &QRCanvas::QRMouseWheelMoved,
            [this](float val, float x, float y, QRModifiers m) {presenter->scale(-val);});

    connect(addBtn, &QPushButton::clicked, [this]() {presenter->addCube( 10);});
    connect(undoBtn, &QPushButton::clicked, [this]() {presenter->undo();});
}


#include "MainWindow.h"

using namespace std;

void MainWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

MainWindow::~MainWindow() {
    presenter.reset();
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setDarkTheme();

    canvas = sptr<QRCanvas> (new QRCanvas(512, 512, this));

    hmap = sptr<QRCanvas> (new QRCanvas(128, 128, this));


    moveRad = new QRadioButton("move", this);
    rotateRad = new QRadioButton("rotate", this);
    scaleRad = new QRadioButton("scale", this);

    view1Rad = new QRadioButton("обзорная камера", this);
    view2Rad = new QRadioButton("прогулочная камера", this);

    settingsLabel = new QRLabel("настройки", 80, this);
    drawTimeLabel = new QRLabel("время отрисовки:", 500,this);
    cameraLabel = new QRLabel("\nкамера:", 80,this);

    erosionLabel = new QRLabel("\nводная эрозия:", 120,this);

    waterCheckBox = new QCheckBox("вода", this);

    g1 = new QButtonGroup(this);
    g2 = new QButtonGroup(this);

    undoBtn = new QPushButton("откатить", this);
    erosionStart = new QPushButton("старт", this);
    erosionEnd = new QPushButton("стоп", this);

    ui = new QRLayoutManager("global", QRHor);
    ui->addLayers("left right", QRVert);
    ui->addWidgets({{"canvas", canvas.get()}, {"label", drawTimeLabel}}, "left");

    ui->goToPath("right");
    ui->addLayers("hmap settings  camera erosion", QRVert);

    ui->addWidgets({{"canvas", hmap.get()}}, "hmap");

    ui->addLayers("label move scale rotate undo water", QRHor, "settings");
    ui->addWidgets({{"radio", moveRad}}, "settings/move");
    ui->addWidgets({{"radio", scaleRad}}, "settings/scale");
    ui->addWidgets({{"radio", rotateRad}}, "settings/rotate");
    ui->addWidgets({{"label", settingsLabel}}, "settings/label");
    ui->addWidgets({{"undo", undoBtn}}, "settings/undo");
    ui->addWidgets({{"water", waterCheckBox}}, "settings/water");

    ui->addLayers("label view1 view2", QRHor, "camera");
    ui->addWidgets({{"radio", view1Rad}}, "camera/view1");
    ui->addWidgets({{"radio", view2Rad}}, "camera/view2");
    ui->addWidgets({{"label", cameraLabel}}, "camera/label");

    ui->addLayers("label buttons", QRHor, "erosion");
    ui->addWidgets({{"label", erosionLabel}}, "erosion/label");
    ui->addWidgets({{"start", erosionStart}, {"end", erosionEnd}}, "erosion/buttons");

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

    waterCheckBox->setChecked(true);
    rotateRad->setChecked(true);

    undoBtn->setFixedWidth(64);

    settingsLabel->setObjectName("headerLabel");

    g1->addButton(moveRad);
    g1->addButton(scaleRad);
    g1->addButton(rotateRad);

    g2->addButton(view2Rad);
    g2->addButton(view1Rad);
}

void MainWindow::addLogic() {
    erosionTimer.setInterval(1000 / erosionFPS);

    connect(canvas.get(), &QRCanvas::QRKeyPressed,
            [this](QRKey k, QRModifiers m) {presenter->transform(k);});
    connect(canvas.get(), &QRCanvas::QRMouseWheelMoved,
            [this](float val, float x, float y, QRModifiers m) {presenter->scale(-val);});

    connect(undoBtn, &QPushButton::clicked, [this]() {presenter->undo();});

    connect(view1Rad, &QRadioButton::clicked, [this]() {presenter->changeCamera();});
    connect(view2Rad, &QRadioButton::clicked, [this]() {presenter->changeCamera();});

    connect(waterCheckBox, &QCheckBox::clicked, [this]() {presenter->setWaterVisible();});

    connect(erosionStart, &QPushButton::clicked, [this]() {erosionTimer.start();});
    connect(erosionEnd, &QPushButton::clicked, [this]() {erosionTimer.stop();});
    connect(&erosionTimer, &QTimer::timeout, [this]() {presenter->erosionIteration();});
}


#include "EditorWindow.h"

using namespace std;

void EditorWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

EditorWindow::~EditorWindow() {
    presenter.reset();
}

EditorWindow::EditorWindow(ModelInitData data, QWidget *parent)
        : QMainWindow(parent) {
    setDarkTheme();

    canvas = sptr<QRCanvas> (new QRCanvas(600, 600, this));

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
    shadesCheckBox = new QCheckBox("тени", this);

    g1 = new QButtonGroup(this);
    g2 = new QButtonGroup(this);

    undoBtn = new QPushButton("откатить", this);
    erosionStart = new QPushButton("старт", this);
    erosionEnd = new QPushButton("стоп", this);
    scaleGrid = new QPushButton("diamond-square", this);
    process = new QPushButton("process", this);
    save = new QPushButton("save", this);

    ui = new QRLayoutManager("global", QRHor);
    ui->addLayers("left right", QRVert);
    ui->addWidgets({{"canvas", canvas.get()}, {"label", drawTimeLabel}}, "left");

    ui->goToPath("right");
    ui->addLayers("hmap settings  camera erosion", QRVert);

    ui->addWidgets({{"canvas", hmap.get()}}, "hmap");

    ui->addLayers("label move scale rotate undo check scaleGrid process", QRHor, "settings");
    ui->addWidgets({{"radio", moveRad}}, "settings/move");
    ui->addWidgets({{"radio", scaleRad}}, "settings/scale");
    ui->addWidgets({{"radio", rotateRad}}, "settings/rotate");
    ui->addWidgets({{"label", settingsLabel}}, "settings/label");
    ui->addWidgets({{"undo", undoBtn}, {"save", save}}, "settings/undo");
    ui->addWidgets({{"scale", scaleGrid}}, "settings/scaleGrid");
    ui->addWidgets({{"process", process}}, "settings/process");
    ui->addWidgets({{"water", waterCheckBox}, {"shades", shadesCheckBox}}, "settings/check");

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
    presenter = sptr<EditorPresenter>(new EditorPresenter(data, *this));
}

void EditorWindow::decorate() {
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

void EditorWindow::addLogic() {
    erosionTimer.setInterval(1000 / erosionFPS);

    connect(canvas.get(), &QRCanvas::QRKeyPressed,
            [this](QRKey k, QRModifiers m) {presenter->transform(k);});
    connect(canvas.get(), &QRCanvas::QRMouseWheelMoved,
            [this](float val, float x, float y, QRModifiers m) {presenter->scale(-val);});
    connect(canvas.get(), &QRCanvas::QRMouseMoved,
            [this](float dx, float dy, QRModifiers m) {if (m.mouseLeftPressed)
                presenter->transformMouse(dx, dy);});

    connect(undoBtn, &QPushButton::clicked, [this]() {presenter->undo();});
    connect(scaleGrid, &QPushButton::clicked, [this]() {presenter->scaleGrid();});
    connect(process, &QPushButton::clicked, [this]() {presenter->process();});

    connect(view1Rad, &QRadioButton::clicked, [this]() {presenter->changeCamera();});
    connect(view2Rad, &QRadioButton::clicked, [this]() {presenter->changeCamera();});

    connect(waterCheckBox, &QCheckBox::clicked, [this]() {presenter->setWaterVisible();});
    connect(shadesCheckBox, &QCheckBox::clicked, [this]() {presenter->setShadesVisible();});

    connect(erosionStart, &QPushButton::clicked, [this]() {erosionTimer.start();});
    connect(erosionEnd, &QPushButton::clicked, [this]() {erosionTimer.stop();});
    connect(&erosionTimer, &QTimer::timeout, [this]() {presenter->erosionIteration();});
    connect(save, &QPushButton::clicked, [this]() {presenter->save();});
}


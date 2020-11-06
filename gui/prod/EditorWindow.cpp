#include "EditorWindow.h"

using namespace std;

void EditorWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

EditorWindow::~EditorWindow() {
    presenter.reset();
}

EditorWindow::EditorWindow(ModelInitData data, string initFacade, QWidget *parent)
        : QMainWindow(parent) {
    setDarkTheme();

    canvas = sptr<QRCanvas> (new QRCanvas(600, 600, this));

    canvasMini = sptr<QRCanvas> (new QRCanvas(150, 150, this));

    drawTimeLabel = new QRLabel("время отрисовки:", 500,this);

    miniScreenHmap = new QCheckBox("карта высот", this);
    miniScreenPlates = new QCheckBox("границы плит", this);
    miniScreenWater = new QCheckBox("водная карта", this);
    miniScreenX = new QRLabel("\nx: 0", 60,this);
    miniScreenY = new QRLabel("\ny: 0", 60,this);
    miniScreenZ = new QRLabel("\nz: 0", 60,this);

    multiRadio = new QRadioField("карта_высот вода разное", 320, this);

    ui = new QRLayoutManager("global", QRHor);
    ui->addLayers("left right", QRVert);
    ui->addWidgets({{"canvas", canvas.get()}, {"label", drawTimeLabel}}, "left");

    ui->goToPath("right");
    ui->addLayers("miniMap big_radio hmap water misc", QRVert);

    ui->addWidgets({{"radio", multiRadio}}, "big_radio");

    // minimap
    ui->addLayers("coords image", QRHor, "miniMap");
    ui->addLayers("canvas boxes", QRVert, "miniMap/image");
    ui->addWidgets({{"x", miniScreenX},
                    {"y", miniScreenY},
                    {"z", miniScreenZ},}, "miniMap/coords");
    ui->addWidgets({{"miniMap", canvasMini.get()}}, "miniMap/image/canvas");
    ui->addWidgets({{"hmap", miniScreenHmap}, {"water", miniScreenWater},
                    {"plates", miniScreenPlates}}, "miniMap/image/boxes");


    mainWidget = new QWidget();
    mainWidget->setLayout(ui->getRootLayout());
    setCentralWidget(mainWidget);

    decorate();
    addLogic();

    presenter = sptr<EditorPresenter>(new EditorPresenter(*this, data, initFacade.empty()));
    if (!initFacade.empty())
        presenter->facade->load(initFacade);

    menu = new QRMenu(presenter->facade, this);
    setMenuBar(menu);

    actionManager =  sptr<ActionManager>(new ActionManager(presenter->facade, presenter.get()));

    ui->goToPath("right", true);

    waterWidget = new EditorWaterWidget(presenter->facade, actionManager, this);
    ui->addWidgets({{"water", waterWidget}}, "water");
    multiRadio->addWidgets({waterWidget}, "вода");

    hmapWidget = new EditorHMapWidget(presenter->facade, actionManager, waterWidget, this);
    ui->addWidgets({{"hmap", hmapWidget}}, "water");
    multiRadio->addWidgets({hmapWidget}, "карта_высот");

    miscWidget = new EditorMiscWidget(presenter->facade, this);
    ui->addWidgets({{"misc", miscWidget}}, "misc");
    multiRadio->addWidgets({miscWidget}, "разное");


    connect(menu, &QRMenu::onQuit, [this](){this->close();});
    connect(miscWidget, &EditorMiscWidget::showFPS, [this](bool x) {
        drawTimeLabel->setVisible(x);
    });
}

void EditorWindow::decorate() {
    auto sepLays = ui->getSepLayers();
    for (auto lay = sepLays.begin(); lay != sepLays.end(); ++lay) {
        (*lay)->addChild(new QRLayoutNode("sep", QRHor, nullptr, new LineSpacer("color:rgb(100,100,100)", 300)));
        (*lay)->getLayout()->setAlignment(Qt::AlignCenter);
    }

    canvas->setToolTip("Управление:\n- клавиши WSAD для перемещения модели (английская раскладка);\n"
                       "- перемещайте мышь с левой кнопкой для вращения модели;\n"
                       "- колесико мыши для масштабирования.");

    canvasMini->setToolTip("Кликните мышкой по миникарте, чтобы получить координаты выбранной точки");


    multiRadio->addWidgets(ui->getAllWidgets("right/misc", true), "разное");


    miniScreenHmap->setChecked(true);
    miniScreenPlates->setChecked(true);
}

void EditorWindow::addLogic() {
    erosionTimer.setInterval(1000 / erosionFPS);

    connect(canvas.get(), &QRCanvas::QRKeyPressed,
            [this](QRKey k, QRModifiers m) {
        presenter->transform(k);
        float x, y, z;
        presenter->defineTransformParams(x,y,z, k);
        actionManager->move(x,y,z);
    });
    connect(canvas.get(), &QRCanvas::QRMouseWheelMoved,
            [this](float val, float x0, float y0, QRModifiers m) {
            float x = 1 + SCALE_FULL_ROTATE_VAL * -val;
            float y = 1 + SCALE_FULL_ROTATE_VAL * -val;
            float z = 1 + SCALE_FULL_ROTATE_VAL * -val;
            actionManager->scale(x, y, z);
    });
    connect(canvas.get(), &QRCanvas::QRMouseMoved,
            [this](float dx, float dy, QRModifiers m) {
        if (m.mouseLeftPressed)
                presenter->transformMouse(dx, dy);
    });

    connect(&erosionTimer, &QTimer::timeout, [this]() {presenter->erosionIteration();});

    connect(miniScreenHmap, &QCheckBox::clicked, [this]() {
        presenter->facade->topDown->setDrawHMap(miniScreenHmap->isChecked());
        presenter->facade->topDown->drawMiniMap();
    });
    connect(miniScreenWater, &QCheckBox::clicked, [this]() {
        presenter->facade->topDown->setDrawWater(miniScreenWater->isChecked());
        presenter->facade->topDown->drawMiniMap();
    });
    connect(miniScreenPlates, &QCheckBox::clicked, [this]() {
        presenter->facade->topDown->setDrawPlates(miniScreenPlates->isChecked());
        presenter->facade->topDown->drawMiniMap();
    });
    connect(canvasMini.get(), &QRCanvas::QRMouseMoved,
            [this](float x, float y, QRModifiers m) {presenter->updateMiniCoords(x, y);});
    connect(canvasMini.get(), &QRCanvas::QRMousePressed,
            [this](float x, float y, QRModifiers m) {presenter->updateMiniCoords(x, y);});
}


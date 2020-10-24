#include "MainWindow.h"

using namespace std;

void MainWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

MainWindow::~MainWindow() {}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setWindowTitle("QRLandscaping");
    setDarkTheme();

    newBtn = new QPushButton("новый проект", this);
    openBtn = new QPushButton("открыть...", this);

    openFile = new QFileDialog(this);

    ui = new QRLayoutManager("global", QRVert);
    //ui->addWidgets({{"new", newBtn}, {"open", openBtn}}, ".");

    //mainWidget = new QWidget();
    //mainWidget->setLayout(ui->getRootLayout());
    //setCentralWidget(mainWidget);


    menu = new QRMenu(this);
    setMenuBar(menu);

    decorate();
    addLogic();
}

void MainWindow::decorate() {
    ui->generateSpacers();

    setFixedWidth(800);
    setFixedHeight(600);

    newBtn->setObjectName("GlobalButton");
    newBtn->setFixedSize(400, 80);
    newBtn->setGeometry(width()/2-newBtn->width()/2, height()/2-newBtn->height(),
            newBtn->width(), newBtn->height());

    openBtn->setObjectName("GlobalButton");
    openBtn->setFixedSize(400, 80);
    openBtn->setGeometry(width()/2-openBtn->width()/2, height()/2+openBtn->height()/2,
                         openBtn->width(), openBtn->height());
}

void MainWindow::addLogic() {
    connect(newBtn, &QPushButton::clicked, [this]() {
        auto *x = new NewModelStartWindow();
        x->show();
        this->close();
    });

    connect(openBtn, &QPushButton::clicked, [this]() {
        string file = openFile->getOpenFileName().toStdString();
        auto *x = new EditorWindow(ModelInitData(), false);
        x->presenter->facade->load(file);
        x->show();
        this->close();
    });
}


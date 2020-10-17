#include "MainWindow.h"

using namespace std;

void MainWindow::setDarkTheme() {
    qApp->setPalette(getDarkTheme());
}

MainWindow::~MainWindow() {}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    setDarkTheme();

    newBtn = new QPushButton("новый ландшафт", this);
    openBtn = new QPushButton("открыть ландшафт", this);

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
    auto sepLays = ui->getSepLayers();
    for (auto lay = sepLays.begin(); lay != sepLays.end(); ++lay) {
        (*lay)->addChild(new QRLayoutNode("sep", QRHor, nullptr, new LineSpacer("color:rgb(100,100,100)", 300)));
        (*lay)->getLayout()->setAlignment(Qt::AlignCenter);
    }

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
        auto *x = new EditorWindow();
        x->show();
        this->close();
    });

    connect(openBtn, &QPushButton::clicked, [this]() {
        string str = openFile->getOpenFileName().toStdString();
        cout << str;
    });
}


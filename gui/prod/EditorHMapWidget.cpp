//
// Created by kurush on 25.10.2020.
//

#include "EditorHMapWidget.h"

EditorHMapWidget::EditorHMapWidget(sptr<Facade> f, sptr<ActionManager> am, EditorWaterWidget *water,
        QWidget *parent): QWidget(parent),
facade(f), manager(am) {
    ui = new QRLayoutManager("global", QRVert);

    auto *iterate = new QPushButton("начать генерацию", this);
    QTimer *processTimer = new QTimer();
    processTimer->setInterval(1000 / hmapFPS);
    connect(iterate, &QPushButton::clicked, [this, iterate, processTimer](){
        if (iterate->text() == "начать генерацию") {
            iterate->setText("остановить");
            processTimer->start();
        }
        else {
            iterate->setText("начать генерацию");
            processTimer->stop();
        }
    });
    connect(processTimer, &QTimer::timeout, [this]() {manager->processHMap();});

    auto *scale = new QPushButton("увеличить плотность сетки", this);
    connect(scale, &QPushButton::clicked, [this, iterate, processTimer, water](){
        if (facade->builder->getWidth() >= maxGridSize) {
            QMessageBox msgBox;
            msgBox.setText("достигнут предел размера сетки. Вы можете увеличить предел в настройках");
            msgBox.exec();
            return;
        }

        if (iterate->text() == "остановить") {
            iterate->setText("начать генерацию");
            processTimer->stop();
        }
        if (water->iterate->text() == "остановить") {
            water->iterate->setText("начать генерацию");
            water->erosionTimer->stop();
        }

        facade->scaleGrid();
        manager->processWater(true);
    });

    //auto setWaterLevel = new QRInputBtn("уровень воды:", "установить", &waterLevel, this,
    //                                    [this](){facade->builder->waterManager->setWaterLevel(waterLevel);});

    //auto addNew = new QPushButton("добавить источник воды", this);
    //connect(addNew, &QPushButton::clicked, [this](){addSource();});
    //addNew->setFixedWidth(200);

    ui->addLayers("check $ data $ scale", QRHor);
    ui->addWidgets({{"btn", iterate}}, "check");
    ui->addWidgets({{"btn", scale}}, "scale");
    //ui->addWidgets({{"input", setWaterLevel}}, "setLevel");
    //ui->addWidgets({{"add", addNew}}, "add");

    scr = new QScrollArea(this);
    //scr->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scr->setWidgetResizable( true );
    scr->setFixedHeight(250);
    lay = new QVBoxLayout();
    w = new QWidget();

    w->setLayout(lay);
    scr->setWidget(w);

    ui->addWidgets({{"sroll", scr}}, "data");

    addMountainSource();
    addHillSource();

    ui->generateSpacers();
    setLayout(ui->getRootLayout());
}
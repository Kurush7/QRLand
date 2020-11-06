//
// Created by kurush on 24.10.2020.
//

#include "EditorWaterWidget.h"

EditorWaterWidget::EditorWaterWidget(sptr<Facade> f, sptr<ActionManager> am, QWidget *parent): QWidget(parent),
facade(f), manager(am) {
    waterLevel = facade->builder->waterManager->getWaterLevel();

    ui = new QRLayoutManager("global", QRVert);

    auto show = new QCheckBox("отображать воду", this);
    connect(show, &QCheckBox::clicked, [this, show]() {facade->setWaterVisible(show->isChecked());});
    show->setChecked(true);

    iterate = new QPushButton("начать генерацию", this);
    erosionTimer = new QTimer();
    erosionTimer->setInterval(1000 / erosionFPS);
    connect(iterate, &QPushButton::clicked, [this](){
        if (iterate->text() == "начать генерацию") {
            iterate->setText("остановить");
            erosionTimer->start();
        }
        else {
            iterate->setText("начать генерацию");
            erosionTimer->stop();
        }
    });
    connect(erosionTimer, &QTimer::timeout, [this]() {manager->processWater();});

    auto setWaterLevel = new QRInputBtn("уровень воды:", "установить", &waterLevel, -maxFloatValidate, maxFloatValidate,
                                        this,[this]() {
                                            facade->builder->waterManager->setWaterLevel(waterLevel);
                                            manager->processWater();
                                        });

    auto addNew = new QPushButton("добавить источник воды", this);
    connect(addNew, &QPushButton::clicked, [this](){addSource();});
    addNew->setFixedWidth(200);

    ui->addLayers("check setLevel $ data $ add", QRHor);
    ui->addWidgets({{"show", show}, {"btn", iterate}}, "check");
    ui->addWidgets({{"input", setWaterLevel}}, "setLevel");
    ui->addWidgets({{"add", addNew}}, "add");

    scr = new QScrollArea(this);
    //scr->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scr->setWidgetResizable( true );
    scr->setFixedHeight(250);
    lay = new QVBoxLayout();
    w = new QWidget();

    w->setLayout(lay);
    scr->setWidget(w);

    ui->addWidgets({{"sroll", scr}}, "data");

    addRainSource();
    loadSources();

    ui->generateSpacers();
    setLayout(ui->getRootLayout());
}
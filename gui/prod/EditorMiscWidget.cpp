//
// Created by kurush on 25.10.2020.
//

#include "EditorMiscWidget.h"

EditorMiscWidget::EditorMiscWidget(sptr<Facade> f, QWidget *parent): QWidget(parent), facade(f) {
    auto shadesCheckBox = new QCheckBox("тени", this);
    auto save = new QPushButton("save", this);

    auto lab = new QRLabel("направление источника света:", this);
    auto xin = new QRInput("x:", &x, this);
    auto yin = new QRInput("y:", &y, this);
    auto zin = new QRInput("z:", &z, this);
    auto lightBtn = new QPushButton("применить", this);

    ui = new QRLayoutManager("global", QRVert);
    ui->addLayers("shades save $ light", QRVert);

    ui->addWidgets({{"shades", shadesCheckBox}}, "shades");
    ui->addWidgets({{"save", save}}, "save");

    ui->addLayers("lab data btn", QRHor, "light");
    ui->addWidgets({{"label", lab}}, "light/lab");
    ui->addWidgets({{"x",xin},{"y",yin},{"z",zin}}, "light/data");
    ui->addWidgets({{"btn", lightBtn}}, "light/btn");


    connect(save, &QPushButton::clicked, [this]() {facade->save();});
    connect(lightBtn, &QPushButton::clicked, [this]() {facade->setLightDir(Vector3D(x,y,z,0));});
    connect(shadesCheckBox, &QCheckBox::clicked, [this,shadesCheckBox]() {
        facade->setShadesVisible(shadesCheckBox->isChecked());
    });

    ui->generateSpacers();
    setLayout(ui->getRootLayout());
}
//
// Created by kurush on 25.10.2020.
//

#include "EditorMiscWidget.h"

EditorMiscWidget::EditorMiscWidget(sptr<Facade> f, QWidget *parent): QWidget(parent), facade(f) {
    ui = new QRLayoutManager("global", QRVert);

    auto shadesCheckBox = new QCheckBox("тени", this);
    auto save = new QPushButton("save", this);

    ui->addWidgets({{"shades", shadesCheckBox}, {"save", save}});

    connect(save, &QPushButton::clicked, [this]() {facade->save();});
    connect(shadesCheckBox, &QCheckBox::clicked, [this,shadesCheckBox]() {
        facade->setShadesVisible(shadesCheckBox->isChecked());
    });

    ui->generateSpacers();
    setLayout(ui->getRootLayout());
}
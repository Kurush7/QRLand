//
// Created by kurush on 18.10.2020.
//

#ifndef BIG3DFLUFFY_QRSETTINGS_H
#define BIG3DFLUFFY_QRSETTINGS_H

#include "QTIncludes.h"
#include "../widgets/QRWidgets.h"
#include "../widgets/layoutManager/QRLayoutManager.h"
#include "QRConstants.h"
#include "configManager.h"

class QRSettings: public QScrollArea {
public:
    QRSettings(QWidget *parent = nullptr): QScrollArea(parent) {
        setWindowTitle("параметры");
        setFixedHeight(400);
        auto w = new QWidget();
        ui = new QRLayoutManager("global", QRVert);
        ui->addLayers("render $ landscape $ water $ climate", QRVert);     // todo sep not working

        auto save = new QPushButton("сохранить конфигурацию", this);
        ui->addLayers("save", QRHor);
        ui->addWidgets({{"save", save}}, "save");
        save->setFixedWidth(200);
        connect(save, &QPushButton::clicked, []() {saveConfig();});

        std::string cur_layer="render";
        QLabel *lab = new QRLabel("Рендеринг");
        lab->setObjectName("headerLabel");
        ui->addWidgets({{"label", lab}}, cur_layer);
        addInput("число потоков", &RENDER_THREAD_CNT, cur_layer);
        addInput("макс. число вершин модели", &MAX_POINT_CNT, cur_layer);

        cur_layer="landscape";
        lab = new QRLabel("Генерация карты высот");
        lab->setObjectName("headerLabel");
        ui->addWidgets({{"label", lab}}, cur_layer);
        addInput("LOD: длина сегмента (число точек)", &FrameSize, cur_layer, true);
        addInput("LOD: макс. ошибка отрисовки", &maxPixelError, cur_layer);
        addInput("LOD: число пикселей на единицу", &pixelsPerUnit, cur_layer);
        addInput("мин. скорость литосферных плит", &minPlateMoveForce, cur_layer);
        addInput("макс. скорость литосферных плит", &maxPlateMoveForce, cur_layer);

        addInput("layerLevelMult", &layerLevelMult, cur_layer);
        addInput("холм: мин. высота (относит.)", &minHillHeightParam, cur_layer);
        addInput("холм: макс. высота (относит.)", &maxHillHeightParam, cur_layer);
        addInput("холм: макс. коэф. затухания", &hillStopCoef, cur_layer);
        addInput("DiamondSquare: сила рандомных изменений (scale)", &diamondSquareRandomCoef, cur_layer);
        addInput("DiamondSquare. сила рандомных изменений (tools)", &diamondSquareRandomUpdateCoef, cur_layer);
        addInput("DiamondSquare: сила рандомных изменений (горы)", &mountainsDSRandomCoef, cur_layer);
        addInput("горы: коэф. высоты", &plateMountainHeightCoef, cur_layer);
        addInput("горы: приоритет интерполяции", &mountainPriorCoef, cur_layer);
        addInput("горы: макс. коэф. затухания", &mountainStopCoef, cur_layer);
        addInput("горы: минимальное напряжение", &mountainsMinTense, cur_layer);

        cur_layer="water";
        lab = new QRLabel("Водная эрозия");
        lab->setObjectName("headerLabel");
        ui->addWidgets({{"label", lab}}, cur_layer);
        addInput("мин. видимый уровень воды", &minimalDrawWaterLevelCoef, cur_layer, true);
        addInput("интервал времени на итерацию", &defaultErosionDT, cur_layer);
        addInput("интенсивность 'капли' дождя", &maxRainDropIntencityCoef, cur_layer);
        addInput("радиус 'капли' дождя", &rainDropRadius, cur_layer);
        addInput("число 'капель' дождя", &rainDropCnt, cur_layer);
        addInput("интенсивность рек", &maxRiverIntencityCoef, cur_layer);
        addInput("объем переносимой почвы", &sedimentCapacity, cur_layer);
        addInput("коэф. размытия", &dissolveConstant, cur_layer);
        addInput("коэф. нанесения", &depositionConstant, cur_layer);
        addInput("мин.уровень наклона", &defaultSlope, cur_layer);
        addInput("коэф. испарения", &evaporationCoef, cur_layer);
        addInput("макс. запас переноса", &fluxPipeCapacity, cur_layer);
        addInput("коэф. гравитации", &gravity, cur_layer);

        cur_layer="climate";
        lab = new QRLabel("Климат");
        lab->setObjectName("headerLabel");
        ui->addWidgets({{"label", lab}}, cur_layer);
        addInput("озеленение: макс. уровень наклона", &maxZSteepCoef, cur_layer, true);

        ui->generateSpacers();

        w->setLayout(ui->getRootLayout());
        setWidget(w);
    }

private:
    QRLayoutManager *ui;

    void addInput(std::string name, float* var, std::string where, bool reset=false) {
        static int cnt=0;
        if (reset) cnt=0;
        std::string l = "input" + std::to_string(cnt);
        ui->addWidgets({{l, new QRInput(name, var, this)}}, where);
    }
    void addInput(std::string name, int* var, std::string where, bool reset=false) {
        static int cnt=0;
        if (reset) cnt=0;
        std::string l = "input" + std::to_string(cnt);
        ui->addWidgets({{l, new QRInput(name, var, this)}}, where);
    }
};



#endif //BIG3DFLUFFY_QRSETTINGS_H

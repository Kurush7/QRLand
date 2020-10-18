//
// Created by kurush on 17.10.2020.
//

#include "SaveLoadManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void SaveLoadManager::save(const sptr<LandscapeBuilder> &builder, string filename) {


    auto hmap = builder->getHeightMap();
    auto water = builder->waterManager->getWaterLevel();
    int32_t w = hmap.width(), h = hmap.height();
    float step = builder->getWorldStep();

    filename += ".qrland";
    FILE *f = fopen(filename.c_str(), "wb");
    for (int i = 0; i < header.size(); ++i)
        fwrite(&header[i], sizeof(char), 1, f);
    fwrite(&step, sizeof(float), 1, f);
    fwrite(&w, sizeof(int32_t), 1, f);
    fwrite(&h, sizeof(int32_t), 1, f);
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            fwrite(&hmap[i][j], sizeof(float), 1, f);
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            fwrite(&water[i][j], sizeof(float), 1, f);
    fclose(f);
}

uptr<LandscapeBuilder> SaveLoadManager::load(std::string filename) {
    int32_t w, h;
    float step;

    FILE *f = fopen(filename.c_str(), "rb");

    // header protection
    char c;
    for (int i = 0; i < header.size(); ++i) {
        fread(&c, sizeof(char), 1, f);
        if (c != header[i])
            return nullptr;
    }

    fread(&step, sizeof(float), 1, f);
    fread(&w, sizeof(int32_t), 1, f);
    fread(&h, sizeof(int32_t), 1, f);

    hmap.resize(w, h);
    water.resize(w, h);

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            fread(&hmap[i][j], sizeof(float), 1, f);
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            fread(&water[i][j], sizeof(float), 1, f);
    fclose(f);

    auto builder = uptr<LandscapeBuilder>(new LandscapeBuilder(w, h, step));
    builder->setHeightMap(hmap);
    return builder;
}

void SaveLoadManager::saveSTL(const sptr<RoamLandscape> &land, string filename){
    QRVector<sptr<QRPolygon3D>> polys;
    land->addMaxDetailPolygons(polys);
    int32_t n = polys.getSize();
    auto points = land->getPurePoints();

    FILE *f = fopen(filename.c_str(), "wb");
    // header
    char x = 0;
    for (int i = 0; i < 80; ++i) fwrite(&x, sizeof(char), 1, f);

    fwrite(&n, sizeof(int32_t), 1, f);

    auto fw = [f](Vector3D v) {
        fwrite(&v[0], sizeof(float), 1, f);
        fwrite(&v[1], sizeof(float), 1, f);
        fwrite(&v[2], sizeof(float), 1, f);
    };

    for (int i = 0; i < n; ++i) {
        QRPolygon3D *p = polys[i].get();
        Vector3D v = len3Norm(p->getNormal());
        fw(v);
        auto ind = p->getPurePointIndexes();

        for (int i = 0; i < 3; ++i) {
            v = points[ind[i]]->getVector();
            fw(v);
        }

        // attribute: color encoding
        QRColor c = p->getTexture()->getColor();
        int16_t attr= c.b/8 + ((c.g/8) << 4) + ((c.r/8) << 12)+(1<<15);
        fwrite(&attr, sizeof(int16_t), 1, f);
    }

    fclose(f);
}
//
// Created by kurush on 17.10.2020.
//

#include "SaveLoadManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void SaveLoadManager::save(const sptr<RoamLandscape> &land, string filename) {

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
        fwrite((const void*)&v[0], sizeof(float), 1, f);
        fwrite((const void*)&v[1], sizeof(float), 1, f);
        fwrite((const void*)&v[2], sizeof(float), 1, f);
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

uptr<RoamLandscape> SaveLoadManager::load(std::string) {

}
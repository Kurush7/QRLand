//
// Created by kurush on 17.10.2020.
//

#ifndef BIG3DFLUFFY_SAVELOADMANAGER_H
#define BIG3DFLUFFY_SAVELOADMANAGER_H

#include "ROAM/RoamLandscape.h"
#include "LandscapeBuilder.h"

class SaveLoadManager {
public:
    // todo! now saves only triangular index polygons
    void save(const sptr<LandscapeBuilder> &builder, std::string filename="landscape");
    void saveSTL(const sptr<RoamLandscape> &land, std::string filename="landscape.stl");
    uptr<LandscapeBuilder> load(std::string);

    QRMatrix<float> hmap, water;
private:
    string header = "QRLandscape Builder Saved File. Protection header";
};


#endif //BIG3DFLUFFY_SAVELOADMANAGER_H

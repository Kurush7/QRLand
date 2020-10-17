//
// Created by kurush on 17.10.2020.
//

#ifndef BIG3DFLUFFY_SAVELOADMANAGER_H
#define BIG3DFLUFFY_SAVELOADMANAGER_H

#include "ROAM/RoamLandscape.h"

class SaveLoadManager {
public:
    // todo! now saves only triangular index polygons
    void save(const sptr<RoamLandscape> &land, std::string filename);
    void saveSTL(const sptr<RoamLandscape> &land, std::string filename="landscape.stl");
    uptr<RoamLandscape> load(std::string);
};


#endif //BIG3DFLUFFY_SAVELOADMANAGER_H

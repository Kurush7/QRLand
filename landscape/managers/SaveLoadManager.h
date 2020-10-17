//
// Created by kurush on 17.10.2020.
//

#ifndef BIG3DFLUFFY_SAVELOADMANAGER_H
#define BIG3DFLUFFY_SAVELOADMANAGER_H

#include "ROAM/RoamLandscape.h"

class SaveLoadManager {
public:
    void save(const sptr<RoamLandscape> &land);
    void saveSTL(const sptr<RoamLandscape> &land);
    uptr<RoamLandscape> load(std::string);
    uptr<RoamLandscape> loadSTL(std::string);
};


#endif //BIG3DFLUFFY_SAVELOADMANAGER_H

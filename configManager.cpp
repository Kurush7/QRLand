//
// Created by kurush on 18.10.2020.
//

#include "configManager.h"

using namespace std;

#define GET_VARIABLE_NAME(Variable) (#Variable)

map<string, float*> getFloatConfigMap();
map<string, int*> getIntConfigMap();

void loadConfig() {
    //todo needed? RENDER_THREAD_CNT = std::thread::hardware_concurrency();
    cout << "Loading config file...";
    map<string, float*> mf = getFloatConfigMap();
    map<string, int*> mi = getIntConfigMap();

    string line;
    fstream infile("config.txt", ios::in);
    while (getline(infile, line)) {
        int x = line.find('=');
        if (x < 0) continue;
        string s = line.substr(0, x);
        float val = atof(line.substr(x+1).c_str());
        if (mf.find(s) != mf.end()) {
            *mf[s] = val;
            cout << "float: " << s << " => " << val << '\n';
        }
        else if (mi.find(s) != mi.end()) {
            *mi[s] = val;
            cout << "int: " << s << " => " << val << '\n';
        }
    }
    infile.close();
}

void saveConfig() {
    cout << "Saving config file...\n";
    map<string, float*> mf = getFloatConfigMap();
    map<string, int*> mi = getIntConfigMap();

    fstream file("config.txt", ios::out);
    for (auto &x: mf)
        file << x.first << "=" << *(x.second) << '\n';
    for (auto &x: mi)
        file << x.first << "=" << *(x.second) << '\n';
    file.close();
}

map<string, float*> getFloatConfigMap() {
    map<string, float*> m;
    m[GET_VARIABLE_NAME(QREPS)] = &QREPS;
    m[GET_VARIABLE_NAME(QRSMALL_EPS)] = &QRSMALL_EPS;
    m[GET_VARIABLE_NAME(QREPS_MINI)] = &QREPS_MINI;
    m[GET_VARIABLE_NAME(QRINF)] = &QRINF;
    m[GET_VARIABLE_NAME(SELECTION_ERROR)] = &SELECTION_ERROR;
    m[GET_VARIABLE_NAME(SCENE_DIST)] = &SCENE_DIST;
    m[GET_VARIABLE_NAME(PROJECTIVE_COEF)] = &PROJECTIVE_COEF;
    m[GET_VARIABLE_NAME(BUTTON_REPRESS_TIME_MSEC)] = &BUTTON_REPRESS_TIME_MSEC;
    m[GET_VARIABLE_NAME(minPlateMoveForce)] = &minPlateMoveForce;
    m[GET_VARIABLE_NAME(maxPlateMoveForce)] = &maxPlateMoveForce;
    m[GET_VARIABLE_NAME(minimalDrawWaterLevelCoef)] = &minimalDrawWaterLevelCoef;
    m[GET_VARIABLE_NAME(defaultErosionDT)] = &defaultErosionDT;
    m[GET_VARIABLE_NAME(maxRainDropIntencityCoef)] = &maxRainDropIntencityCoef;
    m[GET_VARIABLE_NAME(maxRiverIntencityCoef)] = &maxRiverIntencityCoef;
    m[GET_VARIABLE_NAME(sedimentCapacity)] = &sedimentCapacity;
    m[GET_VARIABLE_NAME(dissolveConstant)] = &dissolveConstant;
    m[GET_VARIABLE_NAME(depositionConstant)] = &depositionConstant;
    m[GET_VARIABLE_NAME(defaultSlope)] = &defaultSlope;
    m[GET_VARIABLE_NAME(evaporationCoef)] = &evaporationCoef;
    m[GET_VARIABLE_NAME(fluxPipeCapacity)] = &fluxPipeCapacity;
    m[GET_VARIABLE_NAME(gravity)] = &gravity;
    m[GET_VARIABLE_NAME(maxPixelError)] = &maxPixelError;
    m[GET_VARIABLE_NAME(currentPixelError)] = &currentPixelError;
    m[GET_VARIABLE_NAME(pixelsPerUnit)] = &pixelsPerUnit;
    m[GET_VARIABLE_NAME(layerLevelMult)] = &layerLevelMult;
    m[GET_VARIABLE_NAME(minHillHeightParam)] = &minHillHeightParam;
    m[GET_VARIABLE_NAME(maxHillHeightParam)] = &maxHillHeightParam;
    m[GET_VARIABLE_NAME(hillStopCoef)] = &hillStopCoef;
    m[GET_VARIABLE_NAME(diamondSquareRandomCoef)] = &diamondSquareRandomCoef;
    m[GET_VARIABLE_NAME(diamondSquareRandomUpdateCoef)] = &diamondSquareRandomUpdateCoef;
    m[GET_VARIABLE_NAME(plateMountainHeightCoef)] = &plateMountainHeightCoef;
    m[GET_VARIABLE_NAME(mountainPriorCoef)] = &mountainPriorCoef;
    m[GET_VARIABLE_NAME(mountainStopCoef)] = &mountainStopCoef;
    m[GET_VARIABLE_NAME(mountainsDSRandomCoef)] = &mountainsDSRandomCoef;
    m[GET_VARIABLE_NAME(mountainsMinTense)] = &mountainsMinTense;
    m[GET_VARIABLE_NAME(maxZSteepCoef)] = &maxZSteepCoef;
    m[GET_VARIABLE_NAME(grassMaxHeight)] = &grassMaxHeight;
    m[GET_VARIABLE_NAME(snowMinHeight)] = &snowMinHeight;

    return m;
}

map<string, int*> getIntConfigMap() {
    map<string, int*> m;
    m[GET_VARIABLE_NAME(RENDER_THREAD_CNT)] = &RENDER_THREAD_CNT;
    m[GET_VARIABLE_NAME(MAX_LIGHT_CNT)] = &MAX_LIGHT_CNT;
    m[GET_VARIABLE_NAME(MAX_POINT_CNT)] = &MAX_POINT_CNT;
    m[GET_VARIABLE_NAME(maxPolygonPointCnt)] = &maxPolygonPointCnt;
    m[GET_VARIABLE_NAME(minQuickRenderDataBufferSize)] = &minQuickRenderDataBufferSize;
    m[GET_VARIABLE_NAME(maxNoLODPolygonCnt)] = &maxNoLODPolygonCnt;
    m[GET_VARIABLE_NAME(defaultPlateCnt)] = &defaultPlateCnt;
    m[GET_VARIABLE_NAME(rainDropCnt)] = &rainDropCnt;
    m[GET_VARIABLE_NAME(rainDropRadius)] = &rainDropRadius;
    m[GET_VARIABLE_NAME(FrameSize)] = &FrameSize;
    m[GET_VARIABLE_NAME(maxHillCnt)] = &maxHillCnt;
    m[GET_VARIABLE_NAME(hmapFPS)] = &erosionFPS;
    m[GET_VARIABLE_NAME(erosionFPS)] = &erosionFPS;
    return m;
}
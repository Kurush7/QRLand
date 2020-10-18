//
// Created by kurush on 18.10.2020.
//

#include "loadConfig.h"

using namespace std;

#define GET_VARIABLE_NAME(Variable) (#Variable)

map<string, float*> getConfigMap() {
    map<string, float*> m;
    m[GET_VARIABLE_NAME(RENDER_THREAD_CNT)] = (float*)&RENDER_THREAD_CNT;
    m[GET_VARIABLE_NAME(QREPS)] = (float*)&QREPS;
    m[GET_VARIABLE_NAME(QRSMALL_EPS)] = (float*)&QRSMALL_EPS;
    m[GET_VARIABLE_NAME(QREPS_MINI)] = (float*)&QREPS_MINI;
    m[GET_VARIABLE_NAME(QRINF)] = (float*)&QRINF;
    m[GET_VARIABLE_NAME(SELECTION_ERROR)] = (float*)&SELECTION_ERROR;
    m[GET_VARIABLE_NAME(MAX_LIGHT_CNT)] = (float*)&MAX_LIGHT_CNT;
    m[GET_VARIABLE_NAME(MAX_POINT_CNT)] = (float*)&MAX_POINT_CNT;
    m[GET_VARIABLE_NAME(SCENE_DIST)] = (float*)&SCENE_DIST;
    m[GET_VARIABLE_NAME(PROJECTIVE_COEF)] = (float*)&PROJECTIVE_COEF;
    m[GET_VARIABLE_NAME(maxPolygonPointCnt)] = (float*)&maxPolygonPointCnt;
    m[GET_VARIABLE_NAME(minQuickRenderDataBufferSize)] = (float*)&minQuickRenderDataBufferSize;
    m[GET_VARIABLE_NAME(BUTTON_REPRESS_TIME_MSEC)] = (float*)&BUTTON_REPRESS_TIME_MSEC;
    m[GET_VARIABLE_NAME(maxNoLODPolygonCnt)] = (float*)&maxNoLODPolygonCnt;
    m[GET_VARIABLE_NAME(defaultPlateCnt)] = (float*)&defaultPlateCnt;
    m[GET_VARIABLE_NAME(minPlateMoveForce)] = (float*)&minPlateMoveForce;
    m[GET_VARIABLE_NAME(maxPlateMoveForce)] = (float*)&maxPlateMoveForce;
    m[GET_VARIABLE_NAME(minimalDrawWaterLevelCoef)] = (float*)&minimalDrawWaterLevelCoef;
    m[GET_VARIABLE_NAME(defaultErosionDT)] = (float*)&defaultErosionDT;
    m[GET_VARIABLE_NAME(rainDropIntencityCoef)] = (float*)&rainDropIntencityCoef;
    m[GET_VARIABLE_NAME(rainDropCnt)] = (float*)&rainDropCnt;
    m[GET_VARIABLE_NAME(rainDropRadius)] = (float*)&rainDropRadius;
    m[GET_VARIABLE_NAME(riverIntencityCoef)] = (float*)&riverIntencityCoef;
    m[GET_VARIABLE_NAME(sedimentCapacity)] = (float*)&sedimentCapacity;
    m[GET_VARIABLE_NAME(dissolveConstant)] = (float*)&dissolveConstant;
    m[GET_VARIABLE_NAME(depositionConstant)] = (float*)&depositionConstant;
    m[GET_VARIABLE_NAME(defaultSlope)] = (float*)&defaultSlope;
    m[GET_VARIABLE_NAME(evaporationCoef)] = (float*)&evaporationCoef;
    m[GET_VARIABLE_NAME(fluxPipeCapacity)] = (float*)&fluxPipeCapacity;
    m[GET_VARIABLE_NAME(gravity)] = (float*)&gravity;
    m[GET_VARIABLE_NAME(maxPixelError)] = (float*)&maxPixelError;
    m[GET_VARIABLE_NAME(FrameSize)] = (float*)&FrameSize;
    m[GET_VARIABLE_NAME(pixelsPerUnit)] = (float*)&pixelsPerUnit;
    m[GET_VARIABLE_NAME(layerLevelMult)] = (float*)&layerLevelMult;
    m[GET_VARIABLE_NAME(minHillHeightParam)] = (float*)&minHillHeightParam;
    m[GET_VARIABLE_NAME(maxHillHeightParam)] = (float*)&maxHillHeightParam;
    m[GET_VARIABLE_NAME(hillStopCoef)] = (float*)&hillStopCoef;
    m[GET_VARIABLE_NAME(diamondSquareRandomCoef)] = (float*)&diamondSquareRandomCoef;
    m[GET_VARIABLE_NAME(diamondSquareRandomUpdateCoef)] = (float*)&diamondSquareRandomUpdateCoef;
    m[GET_VARIABLE_NAME(plateMountainHeightCoef)] = (float*)&plateMountainHeightCoef;
    m[GET_VARIABLE_NAME(mountainPriorCoef)] = (float*)&mountainPriorCoef;
    m[GET_VARIABLE_NAME(mountainStopCoef)] = (float*)&mountainStopCoef;
    m[GET_VARIABLE_NAME(mountainsDSRandomCoef)] = (float*)&mountainsDSRandomCoef;
    m[GET_VARIABLE_NAME(mountainsMinTense)] = (float*)&mountainsMinTense;
    m[GET_VARIABLE_NAME(maxZSteepCoef)] = (float*)&maxZSteepCoef;

    return m;
}

void loadConfig() {
    RENDER_THREAD_CNT = std::thread::hardware_concurrency();

    map<string, float*> m = getConfigMap();

    string line;
    fstream infile("config.txt", ios::in);
    while (getline(infile, line)) {
        int x = line.find('=');
        if (x < 0) continue;
        string s = line.substr(0, x);
        float val = atof(line.substr(x+1).c_str());
        if (m.find(s) != m.end()) {
            *m[s] = val;
            cout << s << " => " << val << '\n';
        }
    }
    infile.close();
}
//
// Created by kurush on 01.06.2020.
//

#ifndef BIG3DFLUFFY_QRCONSTANTS_H
#define BIG3DFLUFFY_QRCONSTANTS_H

#include <thread>

extern int RENDER_THREAD_CNT;
extern int shadowScaleCoef;

extern float QREPS;
extern float QRSMALL_EPS;
extern float QREPS_MINI;
extern float QRINF;

extern float SELECTION_ERROR;    // for selection of frame model on click

extern float BUTTON_REPRESS_TIME_MSEC;
extern int hmapFPS;
extern int erosionFPS;

extern int MAX_LIGHT_CNT;
extern int MAX_POINT_CNT;

extern float SCENE_DIST;
extern float PROJECTIVE_COEF;

extern int maxPolygonPointCnt;
extern int minQuickRenderDataBufferSize;

extern int maxNoLODPolygonCnt;
extern int defaultPlateCnt;

//extern QRColor plateBorderColor = QRColor("green");
extern float minPlateMoveForce;
extern float maxPlateMoveForce;

// water
extern float minimalDrawWaterLevelCoef;

extern float defaultErosionDT;

extern float maxRainDropIntencityCoef;
extern int rainDropCnt;
extern int rainDropRadius;

extern float maxRiverIntencityCoef;
extern int maxHillCnt;

extern float sedimentCapacity;
extern float dissolveConstant;
extern float depositionConstant;
extern float defaultSlope;

extern float evaporationCoef;

extern float fluxPipeCapacity;
extern float gravity;

extern float maxPixelError;
extern float currentPixelError;
extern int FrameSize;
extern float pixelsPerUnit;

extern float layerLevelMult;

extern float minHillHeightParam;
extern float maxHillHeightParam;
extern float hillStopCoef;

extern float diamondSquareRandomCoef;
extern float diamondSquareRandomUpdateCoef;

extern float plateMountainHeightCoef;
extern float mountainPriorCoef;
extern float mountainStopCoef;
extern float mountainsDSRandomCoef;
extern float mountainsMinTense;

extern float maxZSteepCoef;
extern float grassMaxHeight;
extern float snowMinHeight;
#endif //BIG3DFLUFFY_QRCONSTANTS_H

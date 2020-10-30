//
// Created by kurush on 18.10.2020.
//

int random_seed = 17;

int RENDER_THREAD_CNT = 4;
int shadowScaleCoef=10;

// math staff
float QREPS = 1e-5;
float QRSMALL_EPS = 1e-9;
float QREPS_MINI = 1e-3;
float QRINF = 1e9;
float SELECTION_ERROR = 1.05;    // for selection of frame model on click

// render stuff
int MAX_LIGHT_CNT = 10;
int MAX_POINT_CNT = 1e6;

float SCENE_DIST = 10.;
float PROJECTIVE_COEF = 0.03;

int maxPolygonPointCnt = 16;
int minQuickRenderDataBufferSize = 1000;

// gui stuff
float BUTTON_REPRESS_TIME_MSEC = 15;
int hmapFPS = 2;
int erosionFPS = 30;

// landscape
int maxNoLODPolygonCnt = 10000;
int defaultPlateCnt = 16;

// QRColor plateBorderColor = QRColor("green");
float minPlateMoveForce = 0.6;
float maxPlateMoveForce = 0.6;

// water
float minimalDrawWaterLevelCoef = 0.1;

float defaultErosionDT = 0.1;

float maxRainDropIntencityCoef = 0.1;
int rainDropCnt = 5; // todo change to per-area drop-cnt
int rainDropRadius = 1;

float maxRiverIntencityCoef = 0.5;
int maxHillCnt = 10;

float sedimentCapacity = 0.02;
float dissolveConstant = 0.02;
float depositionConstant = 0.02;
float defaultSlope = 0.1;

float evaporationCoef = 0.002;

float fluxPipeCapacity = 1;
float gravity = 1;

// roam
// todo add minimal visible level
float maxPixelError = 0.2;
float currentPixelError = 0.2;
int FrameSize = 9;
float pixelsPerUnit = 200;

// tools
float layerLevelMult = 0.3;

float minHillHeightParam = 5;
float maxHillHeightParam = 20;
float hillStopCoef = 4;

float diamondSquareRandomCoef = 0.1;
float diamondSquareRandomUpdateCoef = 0.05;

float plateMountainHeightCoef = 10;
float mountainPriorCoef = 2;
float mountainStopCoef = 4;
float mountainsDSRandomCoef=0.4;
float mountainsMinTense=0.2;

// climate
float maxZSteepCoef = 0.55;
float grassMaxHeight = 2;
float snowMinHeight = 5;
// new line must be here!!!!
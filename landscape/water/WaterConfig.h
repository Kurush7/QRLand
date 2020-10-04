//
// Created by kurush on 25.08.2020.
//

#ifndef BIG3DFLUFFY_WATERCONFIG_H
#define BIG3DFLUFFY_WATERCONFIG_H

const float minimalDrawWaterLevelCoef = 0.1;

const float defaultErosionDT = 0.1;

const float rainDropIntencityCoef = 0.1;
const int rainDropCnt = 5; // todo change to per-area drop-cnt
const int rainDropRadius = 1;

const float riverIntencityCoef = 0.5;

const float sedimentCapacity = 0.02;
const float dissolveConstant = 0.02;
const float depositionConstant = 0.02;
const float defaultSlope = 0.1;

const float evaporationCoef = 0.002;

const float fluxPipeCapacity = 1;
const float gravity = 1;

#endif //BIG3DFLUFFY_WATERCONFIG_H

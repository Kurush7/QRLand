//
// Created by kurush on 25.08.2020.
//

#ifndef BIG3DFLUFFY_WATERCONFIG_H
#define BIG3DFLUFFY_WATERCONFIG_H

const float minimalDrawWaterLevelCoef = QREPS;

const float defaultErosionDT = 0.1;

const float rainDropIntencityCoef = 1;
const int rainDropCnt = 5; // todo change to per-area drop-cnt
const int rainDropRadius = 1;

const float sedimentCapacity = 0.05;
const float dissolveConstant = 0.05;
const float depositionConstant = 0.05;

const float evaporationCoef = 0.01;

const float fluxPipeCapacity = 1;
const float gravity = 1;

#endif //BIG3DFLUFFY_WATERCONFIG_H

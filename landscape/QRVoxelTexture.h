//
// Created by kurush on 05.07.2020.
//

#ifndef BIG3DFLUFFY_QRVOXELTEXTURE_H
#define BIG3DFLUFFY_QRVOXELTEXTURE_H

#include "textures/textures.h"
#include "containers/QRContainers.h"
#include "QRVoxel.h"

QRVector<QRTexture*> initVoxelTextures();

static QRVector<QRTexture*> QRVoxelTextures = initVoxelTextures();



#endif //BIG3DFLUFFY_QRVOXELTEXTURE_H

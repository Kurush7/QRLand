//
// Created by kurush on 05.07.2020.
//

#ifndef BIG3DFLUFFY_QRLANDSCAPETEXTURE_H
#define BIG3DFLUFFY_QRLANDSCAPETEXTURE_H

#include "textures/textures.h"
#include "containers/QRContainers.h"
#include "QRMaterial.h"

QRVector<sptr<QRTexture>> initVoxelTextures();

static QRVector<sptr<QRTexture>> QRTexturesMap = initVoxelTextures();



#endif //BIG3DFLUFFY_QRLANDSCAPETEXTURE_H

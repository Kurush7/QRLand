//
// Created by kurush on 05.07.2020.
//

#include "QRLandscapeTexture.h"

QRVector<sptr<QRTexture>> initVoxelTextures() {
    QRVector<sptr<QRTexture>> textures;
    textures[QRDEFAULT_MATERIAL] = sptr<QRTexture>(new ColorTexture(128,128,128));
    textures[QRVSTONE_MATERIAL] = sptr<QRTexture>(new ColorTexture(128,128,128));
    textures[QRVGRASS_MATERIAL] = sptr<QRTexture>(new ColorTexture(64,128,64));
    textures[QRWATER_MATERIAL] = sptr<QRTexture>(new ColorTexture(32,32,128));
    return textures;
}
//
// Created by kurush on 05.07.2020.
//

#include "QRVoxelTexture.h"

QRVector<QRTexture*> initVoxelTextures() {
    QRVector<QRTexture*> textures;
    textures[QRVSTONE] = new ColorTexture(128,128,128);
    return textures;
}
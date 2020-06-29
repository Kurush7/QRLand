//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRPAINTER_H
#define BIG3DFLUFFY_QRPAINTER_H

#include "objects/QRObject.h"
#include "../3Domain/textures/textures.h"


class QRImage {
public:
    virtual void setPixel(int x, int y, const QRColor &color) = 0;
    virtual void repaint() = 0;
    virtual void refillBg() = 0;
};

class QRImageCreator {
public:
    virtual sptr<QRImageCreator> getImage() {
        if (!image) create();
        return image;
    }
protected:
    virtual void create() = 0;
    sptr<QRImageCreator> image = nullptr;
};


#endif //BIG3DFLUFFY_QRPAINTER_H

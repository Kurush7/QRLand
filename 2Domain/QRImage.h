//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRIMAGE_H
#define BIG3DFLUFFY_QRIMAGE_H

#include "objects/QRObject.h"
#include "../3Domain/textures/textures.h"

class QRImage {
public:
    virtual void setPixel(int x, int y, const QRColor &color) = 0;
    // make mean of current pixel color and given
    virtual void mixPixel(int x, int y, const QRColor &color) = 0;
    virtual const QRColor getPixel(int x, int y) const = 0;
    virtual void repaint() = 0;
    virtual void refillBg() = 0;

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual unsigned char* getData() = 0;
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


#endif //BIG3DFLUFFY_QRIMAGE_H

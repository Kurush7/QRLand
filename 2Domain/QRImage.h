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

    virtual void setBg(const QRColor &color) = 0;
    virtual void refillBg() = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual unsigned char* getData() = 0;
};

class FakeImage: public QRImage {
public:
    FakeImage(int width, int height): width(width), height(height) {}
    virtual void setPixel(int x, int y, const QRColor &color) {}
    // make mean of current pixel color and given
    virtual void mixPixel(int x, int y, const QRColor &color) {}
    virtual const QRColor getPixel(int x, int y) const {return QRColor();}
    virtual void repaint() {}

    virtual void setBg(const QRColor &color) {}
    virtual void refillBg() {}

    virtual int getWidth() const {return width;}
    virtual int getHeight() const {return height;}
    virtual unsigned char* getData() {return nullptr;}  // TODO VEEEEEEEEEERY ACCURATE

private:
    int width, height;
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

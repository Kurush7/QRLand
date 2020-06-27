//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRDEFINES_H
#define BIG3DFLUFFY_QRDEFINES_H

#include <memory>

// todo out from 3Domain up
#define wptr std::weak_ptr
#define sptr std::shared_ptr
#define uptr std::unique_ptr

// todo not good
#define DEFAULT_TEXTURE  sptr<QRTexture>(new ColorTexture("blue"))


#endif //BIG3DFLUFFY_QRDEFINES_H

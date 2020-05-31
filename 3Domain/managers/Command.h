//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_COMMAND_H
#define BIG3DFLUFFY_COMMAND_H

#include "../objects/QRObjects.h"

class BaseCommand {
public:
    std::shared_ptr<Memento> exec();
};


#endif //BIG3DFLUFFY_COMMAND_H

//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_COMMANDMANAGER_H
#define BIG3DFLUFFY_COMMANDMANAGER_H

#include "QRCommandInterface.h"
#include "objects/mementos/CareTaker.h"

class BaseCommandManager {
public:
    BaseCommandManager(sptr<BaseCareTaker> ct): careTaker(ct) {}
    virtual void push(sptr<QRCommand>) = 0;
    virtual bool isEmptyUndo() = 0;
    virtual void exec() = 0;
    virtual void execAll() = 0;
    virtual void undo() = 0;
protected:
    sptr<BaseCareTaker> careTaker;
};


class CommandManager: public BaseCommandManager {
public:
    CommandManager(): BaseCommandManager(sptr<BaseCareTaker> (new CareTaker())) {}
    virtual bool isEmptyUndo() {return careTaker->isEmpty();}
    virtual void push(sptr<QRCommand> c) {queue.push(c);}
    virtual void exec() {
        auto mem = queue.pop()->exec();
        if (mem != nullptr) careTaker->cache(mem);
    }
    virtual void execAll() {
        while(!queue.isEmpty()) exec();
    }
    virtual void undo() {careTaker->restore();}
private:
    QRQueue<sptr<QRCommand>> queue;
};

#endif //BIG3DFLUFFY_COMMANDMANAGER_H

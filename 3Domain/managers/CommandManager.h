//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_COMMANDMANAGER_H
#define BIG3DFLUFFY_COMMANDMANAGER_H

#include "Command.h"
#include "CareTaker.h"

class BaseCommandManager {
public:
    BaseCommandManager(std::shared_ptr<BaseCareTaker> ct): careTaker(ct) {}
    virtual void push(std::shared_ptr<BaseCommand>) = 0;
    virtual bool isEmptyUndo() = 0;
    virtual void exec() = 0;
    virtual void execAll() = 0;
    virtual void undo() = 0;
protected:
    std::shared_ptr<BaseCareTaker> careTaker;
};


class CommandManager: public BaseCommandManager {
public:
    CommandManager(): BaseCommandManager(std::shared_ptr<BaseCareTaker> (new CareTaker())) {}
    virtual bool isEmptyUndo() {return careTaker->isEmpty();}
    virtual void push(std::shared_ptr<BaseCommand> c) {queue.push(c);}
    virtual void exec() {
        auto mem = queue.pop()->exec();
        if (mem != nullptr) careTaker->cache(mem);
    }
    virtual void execAll() {
        while(!queue.isEmpty()) exec();
    }
    virtual void undo() {careTaker->restore();}
private:
    QRQueue<std::shared_ptr<BaseCommand>> queue;
};

#endif //BIG3DFLUFFY_COMMANDMANAGER_H

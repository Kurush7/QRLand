//
// Created by kurush on 25.03.2020.
//

#ifndef KG_THREADFUNCTIONCALL_H
#define KG_THREADFUNCTIONCALL_H

#include <QThread>
#include <functional>

class ThreadFunctionCall : public QThread
{
public:
    ThreadFunctionCall(QObject *parent, std::function<void()> func): QThread(parent), func(func){}
    void run ()
    {
        func();
    }
private:
    std::function<void()> func;
};


#endif //KG_THREADFUNCTIONCALL_H

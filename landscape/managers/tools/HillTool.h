//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_HILLTOOL_H
#define BIG3DFLUFFY_HILLTOOL_H

#include "QRTool.h"
#include "BFSWalk.h"

class HillTool: public QRTool {
public:
    HillTool() {}

    virtual void setToolData(const ToolData &dt);

    virtual bool process() {
        if (height <= 0) return false;
        (*data.hmap)[centerY][centerX] += give;
        height -= give;

        QRQueue<QRPair<int, int>> q;
        q.push({centerX, centerY});

        BFSWalk(q, data.width, data.height, data.hmap);
        return true;
    }

private:
    size_t centerX, centerY;
    float height, give;
};

class HillToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new HillTool());}
};



#endif //BIG3DFLUFFY_HILLTOOL_H

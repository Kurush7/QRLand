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
    virtual void setIntensity(float x) {}

    virtual bool process() {
        if (height <= 0) return false;
        (*data.hmap)[centerY][centerX] += give;
        height -= give;

        QRQueue<QRPair<int, int>> q;
        q.push({centerX, centerY});

        BFSHillWalk(q, data.width, data.height, data.hmap, data.worldStep);
        return true;
    }

private:
    bool inited=false;
    size_t centerX, centerY;
    float height, give;
};

class HillToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new HillTool());}
};

class HillSetTool: public QRTool {
public:
    HillSetTool() {}

    virtual void setToolData(const ToolData &dt);
    virtual void setIntensity(float x) {
        hillCnt = maxHillCnt * x;
        fill();
    }

    virtual bool process() {
        QRVector<HillTool> new_tools;
        for (auto &t: tools) {
            if (t.process())
                new_tools.push_back(t);
        }
        tools = new_tools;
        fill();
        return true;
    }

private:
    void fill() {
        if (tools.getSize() < hillCnt) {
            for (int i = 0; i < hillCnt - tools.getSize(); ++i) {
                tools.push_back(HillTool());
                tools[tools.getSize()-1].setToolData(data);
            }
        }
    }

    bool inited=false;
    int hillCnt = 0;
    QRVector<HillTool> tools;
};

class HillSetToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new HillSetTool());}
};

#endif //BIG3DFLUFFY_HILLTOOL_H

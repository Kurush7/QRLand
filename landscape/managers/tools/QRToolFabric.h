//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_QRTOOLFABRIC_H
#define BIG3DFLUFFY_QRTOOLFABRIC_H

#include <map>

#include "QRTool.h"
#include "LayerTool.h"
#include "FunctionTool.h"
#include "HillTool.h"

enum ToolName {
    FunctionTool, LayerTool, HillTool
};

class QRToolFabric {
public:
    // true if added successfully, false otherwise
    virtual bool registerTool(ToolName name, const sptr<QRToolCreator> &creator);
    virtual uptr<QRTool> create(ToolName name);

private:
    std::map<ToolName, sptr<QRToolCreator>> creators;
};

QRToolFabric initToolFabric();

#endif //BIG3DFLUFFY_QRTOOLFABRIC_H

//
// Created by kurush on 15.04.2020.
//

#ifndef KG_LINECUTTER_H
#define KG_LINECUTTER_H

#include <vector>
#include "Primitives.h"
#include "Figures.h"
#include "AlgoedLine.h"

bool cutLine(const AlgoedLine &, const Rectangle &, AlgoedLine &);

bool cutLine(const AlgoedLine &, const Polygon &, AlgoedLine &);


#endif //KG_LINECUTTER_H

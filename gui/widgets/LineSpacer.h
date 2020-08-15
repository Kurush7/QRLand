//
// Created by kurush on 04.03.2020.
//

#ifndef KG_LINESPACER_H
#define KG_LINESPACER_H

#include <QFrame>
#include <QHBoxLayout>

class LineSpacer: public QFrame {
public:
    LineSpacer(QString = "", int width = 0);
    LineSpacer(const LineSpacer &);
};


#endif //KG_LINESPACER_H

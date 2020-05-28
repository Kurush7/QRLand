//
// Created by kurush on 17.03.2020.
//

#ifndef KG_QRLABEL_H
#define KG_QRLABEL_H

#include <QLabel>

class QRLabel: public QLabel {
public:
    using QLabel::QLabel;
    QRLabel(const QString &text, int width = -1, QWidget *parent = nullptr): QLabel(text, parent) {
        if (width > 0)
            setFixedWidth(width);
    }
};


#endif //KG_QRLABEL_H

//
// Created by kurush on 18.10.2020.
//

#ifndef BIG3DFLUFFY_QRINPUT_H
#define BIG3DFLUFFY_QRINPUT_H

#include <string>

#include "QTIncludes.h"

class QRInput: public QWidget {
public:
    QRInput(std::string name, float *var, QWidget*parent=nullptr, int width=80):
    QWidget(parent), name(name), width(width) {
        varf = var;
        vf = true;
        init();
    }
    QRInput(std::string name, int *var, QWidget*parent=nullptr, int width=80):
    QWidget(parent), name(name), width(width) {
        vari = var;
        vf = false;
        init();
    }

private:
    QLabel *label;
    QLineEdit *edit;
    std::string name;
    float *varf;
    int *vari;
    bool vf;
    int width;

    void init() {
        label = new QLabel(name.c_str(), this);
        edit = new QLineEdit(this);
        edit->setText(QString::number(vf? *varf : *vari));

        auto *lay = new QHBoxLayout();
        lay->addWidget(label);
        lay->addWidget(edit);
        setLayout(lay);

        edit->setFixedWidth(width);

        connect(edit, &QLineEdit::textChanged, [this](const QString &s) {
            bool ok;
            float x = s.toFloat(&ok);
            if (!ok) {
                //todo pop error window
                std::cerr << "field change error!\n";
                return;
            }
            if (vf) *varf = x;
            else *vari = x;
        });
    }
};


#endif //BIG3DFLUFFY_QRINPUT_H

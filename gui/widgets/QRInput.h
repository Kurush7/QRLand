//
// Created by kurush on 18.10.2020.
//

#ifndef BIG3DFLUFFY_QRINPUT_H
#define BIG3DFLUFFY_QRINPUT_H

#include <string>
#include <functional>

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

    QLabel *label;
    QLineEdit *edit;
protected:
    std::string name;
    float *varf;
    int *vari;
    bool vf;
    int width;
    QHBoxLayout *lay;

    void init() {
        label = new QLabel(name.c_str(), this);
        edit = new QLineEdit(this);
        edit->setText(QString::number(vf? *varf : *vari));

        lay = new QHBoxLayout();
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


class QRInputBtn: public QRInput {
public:
    QRInputBtn(std::string name, std::string btn,
            float *var, QWidget *parent = nullptr,
            std::function<void()> f=nullptr, int width = 80) :
            QRInput(name, var, parent, width) {
        auto b = new QPushButton(btn.c_str(), this);
        lay->addWidget(b);

        if (f) {
            connect(b, &QPushButton::clicked, f);
        }
    }

    QRInputBtn(std::string name, std::string btn, int *var, QWidget *parent = nullptr,
               std::function<void()> f=nullptr, int width = 80) :
            QRInput(name, var, parent, width) {
        auto b = new QPushButton(btn.c_str(), this);
        lay->addWidget(b);

        if (f) {
            connect(b, &QPushButton::clicked, f);
        }
    }
};

#endif //BIG3DFLUFFY_QRINPUT_H

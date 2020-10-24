//
// Created by kurush on 23.05.2020.
//

#include "MultiRadioField.h"

using namespace std;

vector<string> split(const string& s, char sep){
    typedef string::const_iterator iter;
    vector<string> ret;
    iter i = s.begin();
    while(i!=s.end()){
        i = find_if(i, s.end(), [sep](char c) {return c != sep;}); // find the beginning of a word
        iter j= find_if(i, s.end(), [sep](char c) {return c == sep;}); // find the end of the same word
        if(i!=s.end()){
            ret.push_back(string(i, j)); //insert the word into vector
            i = j; // repeat 1,2,3 on the rest of the line.
        }
    }
    return ret;
}

QRMultiRadioField::QRMultiRadioField(string names, int width, QWidget *parent): QWidget(parent) {
    auto nameSplit = split(names, ' ');
    int n = nameSplit.size();
    active.clear();

    for (auto name: nameSplit) {
        auto b = new QPushButton(name.c_str(), this);
        b->setFixedWidth(width/n);
        b->setFixedHeight(32);
        b->setObjectName("RadioFieldButton");
        b->setStyleSheet(("QPushButton {background: #303030;}"));

        radios[name] = b;
        lay->addWidget(b);
        nameList.push_back(name);
        active[name] = false;
        connect(b, &QPushButton::clicked, [=] (bool clicked) {
            if (!clicked) {
                this->onClick(name);
            }
        });
    }

    lay->setMargin(0);
    lay->setContentsMargins(0,10,0,0);
    setLayout(lay);

    //active[nameSplit[1]] = true;  // todo if 1 elem; well, we 'change' the value)))
    onClick(nameSplit[0]);
}
void QRMultiRadioField::addWidget(QWidget *w, std::string name) {
    widgets[name].push_back(w);
    w->setVisible(active[name]);
}
void QRMultiRadioField::addWidgets(vector<QWidget*> wids, string name) {
    for (auto w:wids) {
        widgets[name].push_back(w);
        w->setVisible(active[name]);
    }
}

void QRMultiRadioField::onClick(string name, bool artifical) {

    if (active[name] == true)
        radios[name]->setStyleSheet(("QPushButton {background: #303030;}"));
    else
        radios[name]->setStyleSheet(("QPushButton {background: #305090;}"));

    active[name] = !active[name];

    for (auto g: widgets) {
        for (auto w: g.second)
            w->setVisible(active[g.first]);
    }
    //emit onRadioClicked(name);
}
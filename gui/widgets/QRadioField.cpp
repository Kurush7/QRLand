//
// Created by kurush on 23.03.2020.
//

#include "QRadioField.h"

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

QRadioField::QRadioField(std::string names, int width, QWidget *parent): QWidget(parent) {
    auto nameSplit = split(names, ' ');
    int n = nameSplit.size();

    for (auto name: nameSplit) {
        auto b = new QPushButton(name.c_str());
        b->setFixedWidth(width/n);
        b->setFixedHeight(32);
        b->setObjectName("RadioFieldButton");
        b->setStyleSheet(("QPushButton {background: #303030;}"));

        radios[name] = b;
        lay->addWidget(b);
        nameList.push_back(name);
        connect(b, &QPushButton::clicked, [=] (bool clicked) {
            if (!clicked) {
                this->onClick(name);
            }
        });
    }

    lay->setMargin(0);
    lay->setContentsMargins(0,10,0,0);
    setLayout(lay);

    curActive = nameSplit[1];  // well, we 'change' the value)))
    onClick(nameSplit[0]);
    //radios[nameSplit[0]]->setStyleSheet(("QPushButton {background: #305090;}"));
}
void QRadioField::addWidget(QWidget *w, std::string name) {
    widgets[name].push_back(w);
    w->setVisible(curActive == name);
}
void QRadioField::addWidgets(vector<QWidget*> wids, string name) {
    for (auto w:wids) {
        widgets[name].push_back(w);
        w->setVisible(curActive == name);
    }
}

void QRadioField::onClick(string name) {
    if (name == curActive)
        return;

    radios[curActive]->setStyleSheet(("QPushButton {background: #303030;}"));
    radios[name]->setStyleSheet(("QPushButton {background: #305090;}"));

    for (auto w: widgets[curActive])
        w->setVisible(false);
    for (auto w: widgets[name])
        w->setVisible(true);

    curActive = name;
    emit onRadioClicked(name);
}
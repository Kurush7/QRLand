//
// Created by kurush on 23.05.2020.
//

#ifndef KG_MULTIRADIOFIELD_H
#define KG_MULTIRADIOFIELD_H

#include <string>
#include <vector>
#include <map>
#include <functional>

#include <QLayout>
#include <QHBoxLayout>
#include <QPushButton>

class QRMultiRadioField: public QWidget {
    Q_OBJECT
public:
    QRMultiRadioField(std::string, int = 0);
    void addWidget(QWidget*, std::string);
    void addWidgets(std::vector<QWidget*>, std::string);
    bool isActive(std::string s) {return active[s];}
    void changeActive(std::string s) {onClick(s, true);}
    void setActive(std::string s, bool a) {if (isActive(s) != a) onClick(s, true);}

    signals:
        void onRadioClicked(std::string);
private:
    void onClick(std::string, bool artifical = false);

    QHBoxLayout *lay = new QHBoxLayout();
    std::map<std::string, QPushButton*> radios;
    std::map<std::string, std::vector<QWidget*>> widgets;
    std::vector<std::string> nameList;
    std::map<std::string, bool> active;
};


#endif //KG_MULTIRADIOFIELD_H

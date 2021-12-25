//
// Created by kurush on 23.03.2020.
//

#ifndef KG_QRADIOFIELD_H
#define KG_QRADIOFIELD_H

#include <string>
#include <vector>
#include <map>
#include <functional>

#include <QLayout>
#include <QHBoxLayout>
#include <QPushButton>


class QRadioField: public QWidget {
Q_OBJECT
public:
    QRadioField(std::string, int, QWidget *parent=nullptr);
    void addWidget(QWidget*, std::string);
    void addWidgets(std::vector<QWidget*>, std::string);
    std::string getActive() {return curActive;}

signals:
    void onRadioClicked(std::string);
private:
    void onClick(std::string);

    QHBoxLayout *lay = new QHBoxLayout();
    std::map<std::string, QPushButton*> radios;
    std::map<std::string, std::vector<QWidget*>> widgets;
    std::vector<std::string> nameList;
    std::string curActive;
};


#endif //KG_QRADIOFIELD_H

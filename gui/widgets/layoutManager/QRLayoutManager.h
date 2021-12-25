//
// Created by kurush on 17.03.2020.
//

#ifndef KG_QRLAYOUTMANAGER_H
#define KG_QRLAYOUTMANAGER_H

#include <vector>
#include <string>

#include <QLayout>
#include <QPushButton>
#include <QWidget>

#include "QRLayoutNode.h"
#include "../LineSpacer.h"

inline bool space(char c);
inline bool notspace(char c);
std::vector<std::string> split(const std::string& s);

std::vector<std::string> split(const std::string&, char sep = ' ');

class SeparatorGen {
public:
    SeparatorGen(LineSpacer _spacer): spacer(_spacer) {}
private:
    LineSpacer spacer;
};


class QRLayoutManager {
public:
    QRLayoutManager(std::string, LayerType);
    ~QRLayoutManager();

    void generateSpacers() {
        auto sepLays = getSepLayers();
        for (auto lay = sepLays.begin(); lay != sepLays.end(); ++lay) {
            (*lay)->addChild(new QRLayoutNode("sep", QRHor, nullptr, new LineSpacer("color:rgb(100,100,100)", 300)));
            (*lay)->getLayout()->setAlignment(Qt::AlignCenter);
        }
    }

    void addLayers(std::string, LayerType,  std::string = ".", bool isAbsolute = false,
                   bool scrollable=false);
    void addWidgets(std::vector<std::pair<std::string, QWidget*>>,
            std::string path = ".", bool isAbsolute = false);
    void addNode(QRLayoutNode* node, std::string path=".", bool isAbsolute=false);

    QRLayoutNode* getRoot() {return root;}
    QLayout* getRootLayout();
    QLayout* getLayout(std::string, bool isAbsolute = false);
    QWidget* getWidget(std::string, bool isAbsolute = false);
    QRLayoutNode* getNode(std::string, bool isAbsolute = false);
    std::vector<QWidget*> getAllWidgets(std::string, bool);

    void goUp();
    void goToPath(std::string, bool isAbsolute = false);

    std::vector<QRLayoutNode*> getSepLayers();
private:
    QRLayoutNode *root, *cur;
};


#endif //KG_QRLAYOUTMANAGER_H

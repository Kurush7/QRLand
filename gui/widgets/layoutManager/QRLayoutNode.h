//
// Created by kurush on 17.03.2020.
//

#ifndef KG_QRLAYOUTNODE_H
#define KG_QRLAYOUTNODE_H

#include <vector>
#include <string>

#include <QLayout>
#include <QWidget>
#include <QScrollArea>
#include <functional>

#include "QTIncludes.h"

enum LayerType {
    QRHor, QRVert
};

class QRLayoutNode: public QObject {
Q_OBJECT
public:
    QRLayoutNode(std::string, LayerType, QRLayoutNode* = nullptr, QWidget* = nullptr, bool scrollable=false);
    ~QRLayoutNode();

    void addChild(QRLayoutNode*);
    const std::string& getName();
    QRLayoutNode *getPrev();
    QBoxLayout* getLayout();
    QWidget* getWidget();
    QRLayoutNode* getNext(std::string);
    std::map<std::string, QRLayoutNode *> getNext();
    std::vector<QWidget*> getAllWidgets();

    QScrollArea* getScrollArea() {return scr;}

private:
    bool isLeaf = false;
    bool scrollable=false;

    QScrollArea *scr = nullptr;
    QWidget *widget = nullptr;
    QBoxLayout *layout;
    LayerType type;

    std::string name;
    std::map<std::string, QRLayoutNode *> next;
    QRLayoutNode *prev = nullptr;
};


void dfs(QRLayoutNode*, std::function<void(QRLayoutNode*)>);

#endif //KG_QRLAYOUTNODE_H

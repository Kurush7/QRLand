//
// Created by kurush on 17.03.2020.
//

#include "QRLayoutNode.h"

using namespace std;

QRLayoutNode::QRLayoutNode(std::string _name, LayerType _type, QRLayoutNode *prev, QWidget* _widget,
                           bool scrollable)
        : name(_name), type(_type), widget(_widget), prev(prev), scrollable(scrollable) {
    switch (type) {
        case LayerType::QRHor:
            layout = new QHBoxLayout();
            break;
        case LayerType::QRVert:
            layout = new QVBoxLayout();
            break;
    }
    layout->setAlignment(Qt::AlignTop);

    if (widget) {
        isLeaf = true;
        layout->addWidget(widget);
    }
    else if (scrollable) {
        // todo not working
        scr = new QScrollArea();
        auto w = new QWidget();
        layout->addWidget(w);
        layout->addWidget(scr);

        if (type == QRHor) layout = new QHBoxLayout();
        else layout = new QVBoxLayout();

        w->setLayout(layout);
        scr->setWidget(w);
    }
}
QRLayoutNode::~QRLayoutNode() {
    delete layout;
    if (widget)
        delete widget;
    else
        for (auto child = next.begin(); child != next.end(); ++child)
            delete child->second;
}

void QRLayoutNode::addChild(QRLayoutNode *node) {
    if (isLeaf)
        return;
    next[node->getName()] = node;
    node->prev = this;
    layout->addLayout(node->layout);
}

const string& QRLayoutNode::getName() {
    return name;
}

QRLayoutNode* QRLayoutNode::getPrev() {
    return prev;
}

QBoxLayout* QRLayoutNode::getLayout() {
    return layout;
}
QWidget* QRLayoutNode::getWidget() {
    return widget;
}

QRLayoutNode* QRLayoutNode::getNext(std::string name) {
    return next[name];
}

map<string, QRLayoutNode *> QRLayoutNode::getNext() {
    return next;
}

vector<QWidget*>  QRLayoutNode::getAllWidgets() {
    vector<QWidget*> ans;
    for (auto n: next) {
        for (auto x: n.second->getAllWidgets())
            ans.push_back(x);
    }
    if (isLeaf)
        ans.push_back(widget);
    return ans;
}


void dfs(QRLayoutNode* node, std::function<void(QRLayoutNode*)> parse) {
    parse(node);
    for (auto n: node->getNext())
        dfs(n.second, parse);
}
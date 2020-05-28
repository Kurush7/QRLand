//
// Created by kurush on 17.03.2020.
//

#ifndef KG_QRLAYOUTNODE_H
#define KG_QRLAYOUTNODE_H

#include <vector>
#include <string>

#include <QLayout>
#include <QWidget>
#include <functional>

enum LayerType {
    QRHor, QRVert
};

class QRLayoutNode: public QObject {
Q_OBJECT
public:
    QRLayoutNode(std::string, LayerType, QRLayoutNode* = nullptr, QWidget* = nullptr);
    ~QRLayoutNode();

    void addChild(QRLayoutNode*);
    const std::string& getName();
    QRLayoutNode *getPrev();
    QBoxLayout* getLayout();
    QWidget* getWidget();
    QRLayoutNode* getNext(std::string);
    std::map<std::string, QRLayoutNode *> getNext();
    std::vector<QWidget*> getAllWidgets();

private:
    bool isLeaf = false;

    QWidget *widget = nullptr;
    QBoxLayout *layout;
    LayerType type;

    std::string name;
    std::map<std::string, QRLayoutNode *> next;
    QRLayoutNode *prev = nullptr;
};


void dfs(QRLayoutNode*, std::function<void(QRLayoutNode*)>);

#endif //KG_QRLAYOUTNODE_H

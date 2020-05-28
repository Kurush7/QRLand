//
// Created by kurush on 17.03.2020.
//

#include "QRLayoutManager.h"

using namespace std;

vector<string> split1(const string& s, char sep){
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



QRLayoutManager::QRLayoutManager(string name, LayerType type) {
    root = new QRLayoutNode(name, type);
    cur = root;
}

QRLayoutManager::~QRLayoutManager() {
    //delete root;
}

void QRLayoutManager::addLayers(string names, LayerType type, string path, bool isAbsolute) {
    auto old = cur;
    goToPath(path, isAbsolute);

    auto nameSplit = split1(names, ' ');
    int sep_cnt = 0;
    for (auto name: nameSplit) {
        if (name == "$") {
                cur->addChild(new QRLayoutNode("$" + to_string(sep_cnt++), type));
        }
        else
            cur->addChild(new QRLayoutNode(name, type));
    }

    cur = old;
}

void QRLayoutManager::addWidgets(std::vector<std::pair<std::string, QWidget*>> widgets, string path, bool isAbsolute) {
    auto old = cur;
    goToPath(path, isAbsolute);

    for (auto widget: widgets) {
        cur->addChild(new QRLayoutNode(widget.first, LayerType::QRHor, cur, widget.second));
    }
    cur = old;
}

QLayout* QRLayoutManager::getRootLayout() {
    return root->getLayout();
}

QLayout* QRLayoutManager::getLayout(string path, bool isAbsolute) {
    auto old = cur;
    goToPath(path, isAbsolute);

    swap(old, cur);
    return old->getLayout();
}

QWidget* QRLayoutManager::getWidget(string path, bool isAbsolute) {
    auto old = cur;
    goToPath(path, isAbsolute);

    swap(old, cur);
    return old->getWidget();
}

void QRLayoutManager::goUp() {
    cur = cur->getPrev();
}
void QRLayoutManager::goToPath(std::string path, bool isAbsolute) {
    if (isAbsolute)
        cur = root;

    auto pathSplit = split1(path, '/');
    for (auto p: pathSplit) {
        if (p == ".") continue;
        else if (p == "..") goUp();
        else {
            cur = cur->getNext(p);
        }
    }
}

vector<QRLayoutNode*> QRLayoutManager::getSepLayers() {
    vector<QRLayoutNode*> ans;
    int x;
    std::function<void(QRLayoutNode*)> func = [&ans](QRLayoutNode* node) {
        if (node->getName()[0] == '$') ans.push_back(node);
    };
    dfs(root, func);
    return ans;
}

vector<QWidget*> QRLayoutManager::getAllWidgets(string path, bool isAbsolute) {
    auto old = cur;
    goToPath(path, isAbsolute);
    swap(old, cur);

    return old->getAllWidgets();
}
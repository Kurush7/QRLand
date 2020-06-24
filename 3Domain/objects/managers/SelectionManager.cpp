//
// Created by kurush on 01.06.2020.
//

#include "SelectionManager.h"

bool SelectionManager::isSelected(const ObjectIterator &it) {
    for (int i = 0; i < selection.len(); ++i) {
        if (selection[i] == it)
            return true;
    }
    return false;
}

bool SelectionManager::isEmptySelection() {return selection.isEmpty();}

void SelectionManager::switchSelection(const ObjectIterator &it) {
    for (int i = 0; i < selection.len(); ++i) {
        if (selection[i] == it) {
            it->get()->setSelected(false);
            selection.pop(i);
            return;
        }
    }
    it->get()->setSelected(true);
    selection.push_back(it);
}

QRVector<sptr<QRObject>> SelectionManager::getSelection() {
    QRVector<sptr<QRObject>> ans;
    for (auto s: selection)
        ans.push_back(*s);
    return ans;
}
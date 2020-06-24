//
// Created by kurush on 01.06.2020.
//

#ifndef BIG3DFLUFFY_SELECTIONMANAGER_H
#define BIG3DFLUFFY_SELECTIONMANAGER_H

#include "objects/QRObject.h"

class BaseSelectionManager {
public:
    virtual bool isEmptySelection() = 0;
    virtual bool isSelected(const ObjectIterator &it) = 0;
    virtual void switchSelection(const ObjectIterator &it) = 0;
    virtual QRVector<sptr<QRObject>> getSelection() = 0;
};

class SelectionManager: public BaseSelectionManager {
public:
    SelectionManager (const QRVector<sptr<QRObject>> &vector): vector(vector) {}
    virtual bool isSelected(const ObjectIterator &it) {
        for (int i = 0; i < selection.len(); ++i) {
            if (selection[i] == it)
                return true;
        }
        return false;
    }
    virtual void switchSelection(const ObjectIterator &it) {
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
    virtual bool isEmptySelection() {return selection.isEmpty();}
    virtual QRVector<sptr<QRObject>> getSelection() {
        QRVector<sptr<QRObject>> ans;
        for (auto s: selection)
            ans.push_back(*s);
        return ans;
    }
private:
    const QRVector<sptr<QRObject>> &vector;
    QRVector<ObjectIterator> selection;
};

#endif //BIG3DFLUFFY_SELECTIONMANAGER_H

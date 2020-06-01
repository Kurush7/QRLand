//
// Created by kurush on 01.06.2020.
//

#ifndef BIG3DFLUFFY_SELECTIONMANAGER_H
#define BIG3DFLUFFY_SELECTIONMANAGER_H

#include "../BaseObject.h"

class BaseSelectionManager {
public:
    virtual void switchSelection(const ObjectIterator &it) = 0;
    virtual QRVector<shared_ptr<BaseObject>> getSelection() = 0;
};

class SelectionManager: public BaseSelectionManager {
public:
    SelectionManager (const QRVector<shared_ptr<BaseObject>> &vector): vector(vector) {}
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
    virtual QRVector<shared_ptr<BaseObject>> getSelection() {
        QRVector<shared_ptr<BaseObject>> ans;
        for (auto s: selection)
            ans.push_back(*s);
        return ans;
    }
private:
    const QRVector<shared_ptr<BaseObject>> &vector;
    QRVector<ObjectIterator> selection;
};

#endif //BIG3DFLUFFY_SELECTIONMANAGER_H

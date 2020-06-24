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
    virtual bool isSelected(const ObjectIterator &it);
    virtual void switchSelection(const ObjectIterator &it);
    virtual bool isEmptySelection();
    virtual QRVector<sptr<QRObject>> getSelection();
private:
    const QRVector<sptr<QRObject>> &vector;
    QRVector<ObjectIterator> selection;
};

#endif //BIG3DFLUFFY_SELECTIONMANAGER_H

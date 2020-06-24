//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRFRAME3D_H
#define BIG3DFLUFFY_QRFRAME3D_H


#include "../interfaces/QRComposite.h"
#include "../../components/interfaces/QRPoint3D.h"
#include "../../components/interfaces/QREdge3D.h"

class QRFrame3D: public QRComposite {
public:
    explicit QRFrame3D(QRVector<sptr<QRObject>> &obj): QRComposite(obj) {
        p_frame = sptr<QRFrame3D>(this, [](void *ptr){});
    }
    ~QRFrame3D() {p_frame.reset();}

    virtual void acceptVisitor(sptr<QRVisitor> visitor) {visitor->visitFrame3D(p_frame);}

    sptr<QRFrame3D> getPointer() {return p_frame;}
    virtual uptr<QRMemento> save() override;

    virtual bool operator==(const QRFrame3D &b) const = delete;
    virtual QRFrame3D& operator=(const QRFrame3D &p) = delete;
private:
    sptr<QRFrame3D> p_frame;
};



#endif //BIG3DFLUFFY_QRFRAME3D_H

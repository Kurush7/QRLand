//
// Created by kurush on 29.05.2020.
//

#ifndef KG_QRPAIR_H
#define KG_QRPAIR_H

#include <initializer_list>

template<typename F, typename S>
class QRPair {
public:
    QRPair(const F &f, const S &s): fst(f), snd(s) {}

    F fst;
    S snd;
};

template<typename F, typename S>
QRPair<F,S> make_pair(const F &f, const S &s) {return QRPair<F,S>(f, s);}

#endif //KG_QRPAIR_H
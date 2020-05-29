//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_VECTORERROR_H
#define LAB2_VECTORERROR_H

#include "../QRContainerException.h"

class VectorError: public QRContainerException {
public:
    explicit VectorError(const char* file, int line, const char* time, const char* msg);

    virtual ~VectorError();
    virtual const char* what() const noexcept override;
protected:
    char* errorMsg;
};


#endif //LAB2_VECTORERROR_H

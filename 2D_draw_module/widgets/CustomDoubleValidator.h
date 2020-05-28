//
// Created by kurush on 05.03.2020.
//

#ifndef KG_CUSTOMDOUBLEVALIDATOR_H
#define KG_CUSTOMDOUBLEVALIDATOR_H

#include <QDoubleValidator>
#include <QStringList>

class CustomDoubleValidator : public QDoubleValidator{
    QStringList _decimalPoints;
public:
    CustomDoubleValidator(double bottom, double top, int decimals, QObject *parent = Q_NULLPTR)
    : QDoubleValidator(bottom, top, decimals, parent)
    {
        _decimalPoints.append(".");
        _decimalPoints.append(",");
        _decimalPoints.append("comma");
    }
    State validate(QString &str, int &pos) const{
        QString s(str);

        for(QStringList::ConstIterator point = _decimalPoints.begin(); point != _decimalPoints.end(); ++point){
            s.replace(*point, locale().decimalPoint());
        }
        //return QDoubleValidator::validate(s, pos);
        if (s.isEmpty() || (s.startsWith("-") && bottom() < 0 && s.length() == 1)) {
            // allow empty field or standalone minus sign
            return QValidator::Intermediate;
        }
        // check length of decimal places
        QChar point = locale().decimalPoint();
        if(s.indexOf(point) != -1) {
            int lengthDecimals = s.length() - s.indexOf(point) - 1;
            if (lengthDecimals > decimals()) {
                return QValidator::Invalid;
            }
        }
        // check range of value
        bool isNumber;
        double value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }

};


#endif //KG_CUSTOMDOUBLEVALIDATOR_H

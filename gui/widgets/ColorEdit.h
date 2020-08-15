//
// Created by kurush on 03.03.2020.
//

#ifndef KG_COLOREDIT_H
#define KG_COLOREDIT_H


#include <QPushButton>
#include <QColorDialog>

class ColorEdit: public QPushButton
{
Q_OBJECT
public:
ColorEdit( QWidget* parent=nullptr );

void setColor( const QColor& color );
const QColor& getColor();

public slots:
void updateColor();
void changeColor();

private:
QColor color;
};
#endif //KG_COLOREDIT_H

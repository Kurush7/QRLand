//
// Created by kurush on 03.03.2020.
//

#include "ColorEdit.h"

#include <QColorDialog>

ColorEdit::ColorEdit( QWidget* parent )
{
    connect( this, SIGNAL(clicked()), this, SLOT(changeColor()) );
}

void ColorEdit::updateColor()
{
    setStyleSheet( "background-color: " + color.name() );
}

void ColorEdit::changeColor()
{
    QColor newColor = QColorDialog::getColor(color,parentWidget());
    if ( newColor != color )
    {
        setColor( newColor );
    }
}

void ColorEdit::setColor( const QColor& color )
{
    this->color = color;
    updateColor();
}

const QColor& ColorEdit::getColor()
{
    return color;
}
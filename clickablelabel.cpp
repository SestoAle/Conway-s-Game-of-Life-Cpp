#include "clickablelabel.h"
#include <QLabel>
#include <iostream>
#include <QMouseEvent>


ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent)
{

}

void ClickableLabel::mousePressEvent(QMouseEvent* ev)
{
    emit(clickEvent(ev->x(), ev->y()));
}

void ClickableLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit(moveLeftEvent(ev->x(), ev->y()));
    }
    else
    {
        emit(moveRightEvent(ev->x(), ev->y()));
    }

}

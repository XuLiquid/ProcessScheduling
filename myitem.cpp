#include "myitem.h"
#include<QGraphicsItem>
#include<QGraphicsSceneEvent>
#include<QPen>
#include<QDebug>

MyItem::MyItem()
{

    setFlag(QGraphicsRectItem::ItemIsFocusable);
    QPen pen;
    pen.setColor(QColor(118, 121, 124));
    pen.setWidth(1);
    setPen(pen);
    setBrush(QBrush(QColor(5, 184, 204)));
}


void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ;
}

#ifndef MYITEM_H
#define MYITEM_H
#include<QGraphicsItem>
#include<QGraphicsSceneEvent>

class MyItem:public QGraphicsRectItem
{
public:
    MyItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYITEM_H

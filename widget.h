#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <task.h>
#include <process.h>
#include <QList>
#include<QGraphicsScene>
#include<QGraphicsItem>
#include<myitem.h>
#include<space.h>
#include<proc.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_createTask_clicked();
    void onTimeOut();

    void on_proc_table_cellClicked(int row, int column);


    void on_hang_table_cellClicked(int row, int column);

private:
    QGraphicsScene* Scene = new QGraphicsScene();
    QTimer *time;
    Ui::Widget *ui;
    QList<task> backingList;
    QList<proc> waitingList;
    QList<proc> hangingList;

    QList<space> emptyList;
    void addEmpty(int a, int b);
    QList<space> fullList;
    void insertProcess(task t);
    void run();

    int backList_insert(task t);
};
#endif // WIDGET_H

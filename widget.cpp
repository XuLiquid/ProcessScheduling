#include "widget.h"
#include "ui_widget.h"
#include<task.h>
#include<proc.h>
#include<QString>
#include<QTimer>
#include<myitem.h>
#include<space.h>
#include<QTime>
#include<QGraphicsItem>

static int random;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->priority->setMinimum(1);
    ui->size->setMinimum(1);
    ui->time->setMinimum(1);
    ui->task_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->proc_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hang_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    Scene->setSceneRect(0,0,150,400);
    ui->memory->setScene(Scene);

    time = new QTimer();
    time->setInterval(30);
    connect(time,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    time->start();

    emptyList.push_front(space(0,200));
    emptyList.push_back(space(200,0));
}

Widget::~Widget()
{
    backingList.clear();
    delete ui;
}

int Widget::backList_insert(task t)
{
    int i = 0;
    int length = backingList.length();
    for (i = 0;i < length;i++)
    {
        if(backingList.at(i).priority < t.priority)
        {
            backingList.insert(i, t);
            return i;
        }
    }
    backingList.insert(length, t);
    return length;
}


void Widget::on_createTask_clicked()
{
    int p = ui->priority->value();
    int t = ui->time->value();
    QString n = ui->name->text();
    int s = ui->size->value();
    int row = backList_insert(task(p, t, n, s));
    ui->task_table->insertRow(row);
    ui->task_table->setItem(row, 0, new QTableWidgetItem(n));
    ui->task_table->setItem(row, 1, new QTableWidgetItem(QString("%1").arg(p)));
    ui->task_table->setItem(row, 2, new QTableWidgetItem(QString("%1").arg(s)));
    ui->task_table->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(t)));
}

void Widget::addEmpty(int a, int b) //a内存地址，b空间大小
{

    int length = emptyList.length();
    for (int j = 0;j < length; j++)
    {
        if(a < emptyList.at(j).start)
        {
            emptyList.insert(j, space(a, b));
            break;
        }
    }
    int i = 0;
    while(emptyList.length()>1&&emptyList.at(i+1).length != 0)
    {
        if(emptyList.at(i).start+emptyList.at(i).length == emptyList.at(i+1).start)
        {
            int s = emptyList.at(i).start;
            int l = emptyList.at(i).length+emptyList.at(i+1).length;
            emptyList.replace(i, space(s,l));
            emptyList.removeAt(i+1);
            i--;
        }
        i++;
    }
}

void Widget::insertProcess(task t)
{
    for(int i = 0;i < emptyList.length();i++)
    {
        if(t.size <= emptyList.at(i).length)
        {
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

            waitingList.push_front(proc(random+qrand()%10000, t.priority, t.time, t.name, t.size, emptyList.at(i).start));

            int s_e = emptyList.at(i).start + t.size;
            int d_e = emptyList.at(i).length - t.size;
            if(d_e != 0)
            {
                emptyList.replace(i, space(s_e, d_e));
            }
            else
            {
                emptyList.removeAt(i);
            }
            backingList.pop_front();
            ui->task_table->removeRow(0);
            break;
        }
    }
}



void Widget::run()
{
    waitingList.front().time -= 1;
    waitingList.front().priority -= 1;
    if(waitingList.first().time == 0)
    {
        addEmpty(waitingList.first().address, waitingList.first().size);
        waitingList.pop_front();
    }

    for (int i = 0;i < waitingList.length();i++)
    {
        for(int j = i+1;j < waitingList.length();j++)
        {
            if(waitingList.at(i).priority < waitingList.at(j).priority)
            {
                waitingList.swapItemsAt(i, j);
            }
        }
    }

}

void Widget::onTimeOut()
{
    static int value = 0;
    ui->progressBar->setValue(value++);


    if(value > 100)
    {
        random++;

        if(waitingList.isEmpty() == false)
        {
            run();
        }


        if(backingList.isEmpty() == false)
        {
            insertProcess(backingList.first());
        }



        Scene->clear();
        for(int i = 0;i<ui->proc_table->rowCount();i++)
        {
            ui->proc_table->removeRow(i);
        }
        for (int i = 0;i < waitingList.length();i++)
        {
            MyItem *Rect = new MyItem();
            Rect->setRect(0,waitingList.at(i).address*2,150,waitingList.at(i).size*2);
            Scene->addItem(Rect);
            QGraphicsTextItem* text = new QGraphicsTextItem();
            text->setTextWidth(150);
            text->setPos(0,waitingList.at(i).address*2);
            text->setDefaultTextColor(QColor(0, 0, 0));  // 文本色
            QFont font = text->font();
            font.setPixelSize(waitingList.at(i).size);
            text->setFont(font);
            text->setPlainText(QString("%1").arg(waitingList.at(i).PID));
            Scene->addItem(text);


            int id = waitingList.at(i).PID;
            QString n = waitingList.at(i).name;
            int p = waitingList.at(i).priority;
            int t = waitingList.at(i).time;
            int s = waitingList.at(i).size;
            int a = waitingList.at(i).address;
            ui->proc_table->setRowCount(i+1);
            ui->proc_table->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(id)));
            ui->proc_table->setItem(i, 1, new QTableWidgetItem(n));
            ui->proc_table->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(p)));
            ui->proc_table->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(s)));
            ui->proc_table->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(t)));
            ui->proc_table->setItem(i, 5, new QTableWidgetItem(QString("%1").arg(a)));
            ui->proc_table->setItem(i, 6, new QTableWidgetItem("挂起"));
        }
        for (int i = 0;i < hangingList.length();i++)
        {
            MyItem *Rect = new MyItem();
            Rect->setRect(0,hangingList.at(i).address*2,150,hangingList.at(i).size*2);
            Scene->addItem(Rect);
            QGraphicsTextItem* text = new QGraphicsTextItem();
            text->setTextWidth(150);
            text->setPos(0,hangingList.at(i).address*2);
            text->setDefaultTextColor(QColor(0, 0, 0));  // 文本色
            QFont font = text->font();
            font.setPixelSize(hangingList.at(i).size);
            text->setFont(font);
            text->setPlainText(QString("%1").arg(hangingList.at(i).PID));
            Scene->addItem(text);
        }
        value = 0;
    }
}

void Widget::on_proc_table_cellClicked(int row, int column)
{
    if(column == 6)
    {
        hangingList.push_back(waitingList.at(row));
        waitingList.removeAt(row);
        ui->proc_table->removeRow(row);
        int i = hangingList.length()-1;
        ui->hang_table->setRowCount(hangingList.length());
        int id = hangingList.at(i).PID;
        QString n = hangingList.at(i).name;
        int p = hangingList.at(i).priority;
        int t = hangingList.at(i).time;
        int s = hangingList.at(i).size;
        int a = hangingList.at(i).address;
        ui->hang_table->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(id)));
        ui->hang_table->setItem(i, 1, new QTableWidgetItem(n));
        ui->hang_table->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(p)));
        ui->hang_table->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(s)));
        ui->hang_table->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(t)));
        ui->hang_table->setItem(i, 5, new QTableWidgetItem(QString("%1").arg(a)));
        ui->hang_table->setItem(i, 6, new QTableWidgetItem("解挂"));
    }
}

void Widget::on_hang_table_cellClicked(int row, int column)
{
    if(column == 6)
    {
        waitingList.push_back(hangingList.at(row));
        hangingList.removeAt(row);
        ui->hang_table->removeRow(row);
        int i = waitingList.length()-1;
        ui->proc_table->setRowCount(waitingList.length());
        int id = waitingList.at(i).PID;
        QString n = waitingList.at(i).name;
        int p = waitingList.at(i).priority;
        int t = waitingList.at(i).time;
        int s = waitingList.at(i).size;
        int a = waitingList.at(i).address;
        ui->proc_table->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(id)));
        ui->proc_table->setItem(i, 1, new QTableWidgetItem(n));
        ui->proc_table->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(p)));
        ui->proc_table->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(s)));
        ui->proc_table->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(t)));
        ui->proc_table->setItem(i, 5, new QTableWidgetItem(QString("%1").arg(a)));
        ui->proc_table->setItem(i, 6, new QTableWidgetItem("挂起"));
    }
}

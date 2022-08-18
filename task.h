#ifndef TASK_H
#define TASK_H

#include<QString>
class task
{
public:
    task(int p, int t, QString n, int s);
    int priority;
    int time;
    QString name;
    int size;
};

#endif // TASK_H

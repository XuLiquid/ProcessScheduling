#ifndef PROC_H
#define PROC_H

#include<QString>
class proc
{
public:
    proc(int pid, int p, int t, QString n, int s, int a);
    int PID;
    int priority;
    int time;
    QString name;
    int size;
    int address;
};

#endif // PROC_H

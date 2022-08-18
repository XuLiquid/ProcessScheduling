#include "proc.h"

proc::proc(int pid, int p, int t, QString n, int s, int a)
{
    PID = pid;
    priority = p;
    time = t;
    name = n;
    size = s;
    address = a;
}

#include "TaskPool.h"

TaskPool::TaskPool(QObject *parent) : QObject(parent)
{

}

TaskPool::~TaskPool()
{

}

void TaskPool::run()
{
    qDebug() << "当前运行的线程ID = " << QThread::currentThreadId();
    QThread::sleep(1);
}

#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

class TaskPool : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit TaskPool(QObject *parent = nullptr);
    ~TaskPool();

protected:
    void run() Q_DECL_OVERRIDE;
signals:
    void taskSignal();

private:
    QMutex mutex_;
};

#endif // TASKPOOL_H

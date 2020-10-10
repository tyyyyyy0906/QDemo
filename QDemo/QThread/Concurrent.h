#ifndef CONCURRENT_H
#define CONCURRENT_H

#include <QObject>

class Concurrent : public QObject
{
    Q_OBJECT
public:
    explicit Concurrent(QObject *parent = nullptr);

private slots:
    void start();
signals:

};

#endif // CONCURRENT_H

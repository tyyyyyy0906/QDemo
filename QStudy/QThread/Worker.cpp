/*
 *  Desc  : 这是一个QT元对象系统和MOC编译器的实践Demo;
 *  Author: sjxnhjp@gmail.com
 */

#include <QObject>
#include <QDebug>

#include "Worker.h"

class T final : public QObject
{
    Q_OBJECT
public:
    T(QObject* parent = nullptr) : QObject(parent)
    {

    }

public slots:
    void recivMit()
    {
        qDebug() << "i get it!!!" << QString("0906") << (int)H::LEFT << (int)K::LEFT;
    }
};

#include "Worker.moc"

Basic::Basic(QObject* parent) : QObject(parent)
{
    T* r = new T();

    connect(this, SIGNAL(transMit()), r, SLOT(recivMit()));
}

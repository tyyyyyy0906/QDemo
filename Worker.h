#ifndef WORKER_H
#define WORKER_H

#include <QObject>

// C++11新特性强化可用性（避免重名覆盖，尤其涉及到重载）
enum class H
{
    LEFT = 520,
    RIGHT   ,
    CENTER  ,
    NONE
};

enum class K
{
    LEFT = 1314,
    CENTER   ,
    DEFAULT
};

class Basic : public QObject
{
    Q_OBJECT
public:
    Basic(QObject* parent = Q_NULLPTR);

signals:
    void transMit();
};

#endif // WORKER_H

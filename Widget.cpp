// QT Stardand Libaray
#include <QThread>
#include <QDebug>
#include <QVector>
#include <QObject>

#include <QPushButton>

#include "Widget.h"
#include "Worker.h"
#include "ui_Widget.h"

// C++ Stardand Libaray
#include <algorithm>
// C Stardand Libaray
#include <stdlib.h>

#define RANDOM(x) rand()%(x)

// QThread 第一种使用方法（重写run方法）
class Reciver final : public QThread
{
    Q_OBJECT
public:
    Reciver()
    {
        v_ = {};
    }
public:
    void start()
    {
        qDebug() << "Class Reciver Thread ID = " << QThread::currentThreadId();
        QThread::start();
    }
protected:
    void run() Q_DECL_OVERRIDE
    {
        qDebug() << "Current Thread ID = " << QThread::currentThreadId();
        v_.clear();
        for (int i = 0; i < 10000000; ++i)
        {
            v_.push_back(i);
        }

        for (const auto& value : v_)
        {
            Q_UNUSED(value)
        }

        std::sort(v_.begin(), v_.end(), [](int a, int b) -> bool{ return a < b; });

        qDebug() << "new thread operator finished!!!";

        /*
            说明：因为signal/slot依赖Q_OBJECT宏，而Q_OBJECT宏依赖于QT的元编译器即moc编译器,
            但是QT的moc编译器只会默认查找.h文件中的Q_OBJECT宏而不会去查找.cpp文件中的Q_OBJECT宏。
            如果想要在cpp中引入Q_OBJECT宏必须要引入moc文件（一定是放在文件末尾）告诉moc编译器引用了Q_OBJECT。

            emit sendStartResult();  ERROR
        */
        emit sendStartResult();
    }
private:
    QVector<int> v_;

signals:
    void sendStartResult();
};

// QThread的第二种用法（moveToThread）
class Worker final : public QObject
{
    Q_OBJECT
public:
    Worker(QObject* parent = nullptr) : QObject(parent)
    {
        v_ = {};
    }
public slots:
    void doWork()
    {
        qDebug() << "Worker Thread ID = " << QThread::currentThreadId();
        srand((int)time(0));
        for (int i = 0; i < 10; ++i)
        {
            v_.push_back(RANDOM(100));
        }
        std::sort(v_.begin(), v_.end(), [](int a, int b) -> bool{ return a > b; });

        for (const int& k : v_)
        {
            qDebug() << "print current array ele = " << k;
        }
        emit threadWorkFinished();
    }
signals:
    void threadWorkFinished();

private:
    QVector<int> v_;
};

#include "Widget.moc"

Widget::Widget(QWidget *parent)
      : QWidget(parent)
      , ui(new Ui::Widget)
      , p_(new Reciver)
{
    ui->setupUi(this);

    itemInit();

    connect(ui->pushButton,   SIGNAL(clicked(void)),     this, SLOT(onButtonClicked(void)));
    connect(ui->pushButton_2, SIGNAL(clicked(void)),     this, SLOT(onButtonClickedAnOther(void)));
    connect(p_,               &Reciver::sendStartResult, this, &Widget::reciverFromNewThread, Qt::QueuedConnection);

    Worker* work  = new Worker();
    Basic * basic = new Basic();

    work->moveToThread(&workerThread);
    connect(this,          SIGNAL(startWorker()),        work, SLOT(doWork()));
    connect(work,          SIGNAL(threadWorkFinished()), this, SLOT(reciverWorkThreadFinished()), Qt::QueuedConnection);
    connect(&workerThread, &QThread::finished,           work, &QObject::deleteLater);

    connect(ui->pushButton, SIGNAL(clicked()), basic, SIGNAL(transMit()));
}

Widget::~Widget()
{
    delete ui;
    delete p_;

    workerThread.wait();
    workerThread.quit();
}

void Widget::itemInit()
{
    ui->pushButton->setFont(QFont("微软雅黑"));
    ui->pushButton->setText(QString("开始"));
    ui->pushButton_2->setText(QString("开启Work线程"));
    ui->pushButton_2->setFont(QFont("微软雅黑"));
}

void Widget::onButtonClicked()
{
    ui->pushButton->setText(QString("等待接收中..."));
    ui->pushButton->setEnabled(false);
    p_->start();
    qDebug() << "Main Proccess Thread ID = " << QThread::currentThreadId();
}

void Widget::onButtonClickedAnOther()
{
    ui->pushButton_2->setText(QString("等待Work完成中..."));
    emit this->startWorker();
    ui->pushButton_2->setEnabled(false);
}

void Widget::reciverFromNewThread()
{
    ui->pushButton->setText(QString("哇哦，已经接收到新线程完成的信息了^-^"));
    ui->pushButton->setEnabled(true);
}

void Widget::reciverWorkThreadFinished()
{
    ui->pushButton_2->setText(QString("哇哦，已经接收到Worker线程完成的信息了^-^"));
    ui->pushButton_2->setEnabled(true);
}

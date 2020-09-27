// QT Stardand Libaray
#include <QThread>
#include <QDebug>
#include <QVector>
#include <QObject>

#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>
#include <QThreadPool>
#include <QtConcurrent>
#include <QFuture>

#include "Widget.h"
#include "Worker.h"
#include "TaskPool.h"
#include "ui_Widget.h"

// C++ Stardand Libaray
#include <algorithm>
// C Stardand Libaray
#include <stdlib.h>

#define RANDOM(x) rand()%(x)

class Transfer
{
public:
    Widget* this_ = Q_NULLPTR;

    QMutex mutex_;
};

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
            如果想要在cpp中引入Q_OBJECT宏必须要引入moc文件（一定是放在使用处末尾）告诉moc编译器引用了Q_OBJECT。

            QT编译流程：
            1、QT将源码交与C++标准编译器预编译之前会由moc对C++源文件进行分析。
            2、如果.h文件中包含了Q_OBJECT宏则QT会自动生成一个包含Q_OBJECT实现的源代码C++文件即moc_xxx.cpp。
            3、将生成的moc_xxx.cpp文件加入C++标准编译器中参与编译。

            这样的一个编译流程从而导致QT的编译速度超级慢^_^(原本C++编译速度已经很慢了)。

            #: Notice: moc只会对.h文件中是否存在Q_OBJECT宏进行分析，并不会对.cpp文件分析,如果在.cpp
                       文件中引入了Q_OBJECT宏需要手动引入对应的moc文件即: xxx.moc,否则将报错。

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
    QMutex m_;
public slots:
    void doWork()
    {
        QMutexLocker lock(&m_);
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
      , workerThread(new QThread)
      , tran_(new Transfer)
{
    ui->setupUi(this);

    tran_->this_ = this;

    itemInit();

    connect(ui->pushButton,   SIGNAL(clicked(void)),     this, SLOT(onButtonClicked(void)));
    connect(ui->pushButton_2, SIGNAL(clicked(void)),     this, SLOT(onButtonClickedAnOther(void)));
    connect(p_,               &Reciver::sendStartResult, this, &Widget::reciverFromNewThread, Qt::QueuedConnection);

    Worker* work  = new Worker();
    Basic * basic = new Basic();

    work->moveToThread(workerThread);
    connect(this,          SIGNAL(startWorker()),        work, SLOT(doWork()));
    connect(work,          SIGNAL(threadWorkFinished()), this, SLOT(reciverWorkThreadFinished()), Qt::QueuedConnection);
    connect(workerThread,  &QThread::finished,           work, &QObject::deleteLater);

    connect(ui->pushButton, SIGNAL(clicked()), basic, SIGNAL(transMit()));
    connect(this, &Widget::nameChanged, this, [this]()
    {
        qDebug() << "current m_name has be changed = " << m_name;
    });

    connect(ui->pushButton_3, SIGNAL(clicked(void)), this, SLOT(startTaskTool(void)));
    connect(ui->pushButton_4, SIGNAL(clicked(void)), this, SLOT(startConcurrent(void)));
}

Widget::~Widget()
{
    delete ui;
    delete p_;

    workerThread->wait();
    workerThread->quit();
}

void Widget::itemInit()
{
    ui->pushButton->setFont(QFont("微软雅黑"));
    ui->pushButton_2->setFont(QFont("微软雅黑"));
    ui->pushButton_3->setFont(QFont("微软雅黑"));
    ui->pushButton_4->setFont(QFont("微软雅黑"));
    ui->pushButton->setText(QString("开始"));
    ui->pushButton_2->setText(QString("开启Work线程"));
    ui->pushButton_3->setText(QString("开启任务池"));
    ui->pushButton_4->setText(QString("QtConcurrent高级用法"));
}

void Widget::onButtonClicked()
{
    ui->pushButton->setText(QString("等待接收中..."));
    ui->pushButton->setEnabled(false);
    p_->start();
    m_name = "QThread";
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
    m_name = "QThread";
}

void Widget::reciverWorkThreadFinished()
{
    ui->pushButton_2->setText(QString("哇哦，已经接收到Worker线程完成的信息了^-^"));
    ui->pushButton_2->setEnabled(true);
}

void Widget::startTaskTool()
{
    QThreadPool taskPool;
    taskPool.setMaxThreadCount(5);
    for (int i = 0; i < 5; i++)
    {
        taskPool.start(new TaskPool());
        QThread::sleep(1);
    }
}

QString Widget::conResult(const QString& value)
{
    qDebug("current function thread id = %#x", QThread::currentThreadId());
    return QString("Hello %1").arg(value);
}

void Widget::startConcurrent()
{
    qDebug() << "start Concurrent" << QThread::currentThreadId();
    QFuture<void> f_ = QtConcurrent::run([=]()
    {
        qDebug() << "current thread = " << QThread::currentThreadId();
    });
    QFuture<QString> future = QtConcurrent::run(this, &Widget::conResult, QString("Qt6"));
    future.waitForFinished();
    qDebug() << "get result = " << future.result();
}



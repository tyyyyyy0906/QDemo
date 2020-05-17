#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Reciver;
class Widget : public QWidget
{
    Q_OBJECT
    QThread workerThread;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void itemInit();

public slots:
    void onButtonClicked(void);
    void onButtonClickedAnOther(void);
public slots:
    void reciverFromNewThread(void);
    void reciverWorkThreadFinished(void);

signals:
    void startWorker();

private:
    Ui::Widget *ui;
    Reciver* p_;
};
#endif // WIDGET_H

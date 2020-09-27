#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QFont>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Transfer;
class Reciver;
class Widget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QFont   font MEMBER m_font NOTIFY fontChanged);
    Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged);

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void itemInit();
    QString conResult(const QString& value);
public:
    QFont m_font;
    QString m_name;

public slots:
    void onButtonClicked(void);
    void onButtonClickedAnOther(void);
    void startConcurrent();
public slots:
    void reciverFromNewThread(void);
    void reciverWorkThreadFinished(void);
    void startTaskTool(void);

signals:
    void startWorker();
    void fontChanged();
    void nameChanged();

private:
    Ui::Widget *ui;
    Reciver* p_;
    QThread* workerThread;
    friend class Transfer;
    Transfer* tran_;
};
#endif // WIDGET_H

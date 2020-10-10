#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = 0);
    ~Client();

    void initWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onClientConnectButton();
    void onClientSendMsgButton();
    void onRecvConnectStatus();
    void onReadRecvData();
    void onRecvServerDisConnected();

private:
    Ui::Client *ui;
    QTcpSocket *client_;
};

#endif // CLIENT_H

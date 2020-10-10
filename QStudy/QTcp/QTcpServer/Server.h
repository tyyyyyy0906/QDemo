#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

    void initWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onServerListenerButton();
    void onNewClientConnected();
    void onReadClientData();
    void onClientDisConnected();
    void onServerSendMsgData();

private:
    Ui::Server *ui;
    QTcpSocket *socket_;
    QTcpServer *server_;
};

#endif // SERVER_H

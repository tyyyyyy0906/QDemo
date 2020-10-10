#include <QObject>
#include <QEvent>
#include <QKeyEvent>

#include "Server.h"
#include "ui_Server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
    socket_(new QTcpSocket),
    server_(new QTcpServer)
{
    ui->setupUi(this);
    initWindow();

    connect(ui->server_listener_button, SIGNAL(clicked()), this, SLOT(onServerListenerButton()));
    connect(ui->server_send_button    , SIGNAL(clicked()), this, SLOT(onServerSendMsgData()));

    connect(server_, &QTcpServer::newConnection, this, &Server::onNewClientConnected);
}

Server::~Server()
{
    delete ui;
    delete socket_;
    delete server_;
}

void Server::initWindow()
{
    this->setWindowTitle("Server");
    ui->server_listener_button->setText(QStringLiteral("Listener"));
    ui->server_send_button->setEnabled(false);
    ui->server_sendmsg_edit->setEnabled(false);
    ui->server_recvmsg_edit->setReadOnly(true);
    ui->server_sendmsg_edit->installEventFilter(this);
}

void Server::onServerListenerButton()
{
    if (!server_->isListening())
    {
        int port_ = ui->server_port_edit->text().toInt();
        if (!server_->listen(QHostAddress::Any, port_))
        {
            qDebug() << "current server listener failed = " << server_->errorString();
            return;
        }
        qDebug("start port listener worker!!!");
        ui->server_listener_button->setText(QStringLiteral("Cancel"));
        ui->server_send_button->setEnabled(true);
        ui->server_sendmsg_edit->setEnabled(true);
    }
    else
    {
        socket_->abort();
        server_->close();
        ui->server_listener_button->setText(QStringLiteral("Listener"));
        ui->server_send_button->setEnabled(false);
        ui->server_sendmsg_edit->setEnabled(false);
    }
}

void Server::onNewClientConnected()
{
    qDebug("new client connect success!!!");
    socket_ = server_->nextPendingConnection();

    QObject::connect(socket_, &QTcpSocket::readyRead   , this, &Server::onReadClientData);
    QObject::connect(socket_, &QTcpSocket::disconnected, this, &Server::onClientDisConnected);

    ui->server_send_button->setEnabled(true);
}

void Server::onReadClientData()
{
    QByteArray buffer_;
    buffer_ = socket_->readAll();
    if (buffer_.isEmpty())
    {
        QString context = ui->server_recvmsg_edit->toPlainText();
        context += tr(buffer_);
        qDebug() << "recv result = " << context;
        ui->server_recvmsg_edit->setText(context);
    }
}

void Server::onClientDisConnected()
{
    qDebug("current client has be disconnected");
}

void Server::onServerSendMsgData()
{
    socket_->write(ui->server_sendmsg_edit->toPlainText().toLatin1());
    socket_->flush();
    ui->server_sendmsg_edit->clear();
}

bool Server::eventFilter(QObject *watched, QEvent *event)
{
    quint32    type_ = static_cast<quint32>(event->type());
    QKeyEvent *keys_ = static_cast<QKeyEvent*>(event);

    if (type_ == QEvent::KeyPress && keys_->key() == Qt::Key_Return)
    {
        if (watched == ui->server_sendmsg_edit)
        {
            onServerSendMsgData();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

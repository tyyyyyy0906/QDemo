#include <QMenu>
#include <QAction>
#include <QKeyEvent>

#include "Client.h"
#include "ui_Client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client),
    client_(new QTcpSocket)
{
    ui->setupUi(this);
    initWindow();

    connect(ui->client_connect_button, SIGNAL(clicked()), this, SLOT(onClientConnectButton()));
    connect(ui->client_send_button   , SIGNAL(clicked()), this, SLOT(onClientSendMsgButton()));
    connect(ui->client_recvmsg_edit  , &QTextEdit::customContextMenuRequested, this, [&](const QPoint&)
    {
        qDebug("client customContextMenuRequested");
        QMenu  * menu = new QMenu();
        QAction* copy_      = new QAction("copy"      , menu);
        QAction* select_All = new QAction("select all", menu);
        QAction* clear_     = new QAction("clear"     , menu);
        copy_ ->setShortcut(QKeySequence::Copy);
        clear_->setShortcut(QKeySequence::Delete);
        select_All->setShortcut(QKeySequence::SelectAll);

        connect(copy_, &QAction::triggered, ui->client_recvmsg_edit, [&]
        {
            ui->client_recvmsg_edit->copy();
        });
        connect(clear_, &QAction::triggered, ui->client_recvmsg_edit, [&]
        {
            ui->client_recvmsg_edit->clear();
        });
        connect(select_All, &QAction::triggered, ui->client_recvmsg_edit, [&]
        {
            ui->client_recvmsg_edit->selectAll();
        });

        menu->addAction(copy_ );
        menu->addAction(clear_);
        menu->addAction(select_All);
        menu->move(QCursor::pos());
        menu->show();
    });

    connect(client_, &QTcpSocket::connected   , this, &Client::onRecvConnectStatus);
    connect(client_, &QTcpSocket::readyRead   , this, &Client::onReadRecvData);
    connect(client_, &QTcpSocket::disconnected, this, &Client::onRecvServerDisConnected);
    connect(client_, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                                                      [=](QAbstractSocket::SocketError socketError)
    {
        qDebug() << "current socket connect error result =" << socketError;
    });

}

Client::~Client()
{
    delete ui;
    delete client_;
}

void Client::initWindow()
{
    this->setWindowTitle("Client");
    ui->client_send_button   ->setEnabled(false);
    ui->client_sendmsg_edit  ->setEnabled(false);
    ui->client_recvmsg_edit  ->setEnabled(false);
    ui->client_recvmsg_edit  ->setReadOnly(true);
    ui->client_connect_button->setText(QStringLiteral("connect"));
    ui->client_address_edit  ->setText(QStringLiteral("127.0.0.1"));
    ui->client_recvmsg_edit  ->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->client_port_edit   ->installEventFilter(this);
    ui->client_sendmsg_edit->installEventFilter(this);
}

void Client::onClientConnectButton()
{
    if (client_->state() == QAbstractSocket::UnconnectedState)
    {
        QString clientAddress_ = ui->client_address_edit->text();
        int clientPort_ = ui->client_port_edit->text().toInt();

        client_->abort();
        client_->connectToHost(clientAddress_, clientPort_);

        if (!client_->waitForConnected(30000))
        {
            qDebug("socket connect failed!!!");
            return;
        }
        qDebug("socket connected!!!");

        ui->client_connect_button->setText(QStringLiteral("disconnect"));
        ui->client_send_button->setEnabled(true);
        ui->client_sendmsg_edit->setEnabled(true);
        ui->client_recvmsg_edit->setEnabled(true);
    }
    else
    {
        client_->disconnectFromHost();
        ui->client_connect_button->setText(QStringLiteral("connect"));
        ui->client_send_button->setEnabled(false);
        ui->client_sendmsg_edit->setEnabled(false);
        ui->client_recvmsg_edit->setEnabled(false);
    }
}

void Client::onClientSendMsgButton()
{
    qDebug("start send meesage to server");
    client_->write(ui->client_sendmsg_edit->toPlainText().toLatin1());
    client_->flush();
    ui->client_sendmsg_edit->clear();
}

void Client::onRecvConnectStatus()
{
    qDebug("client connect success!!!");
}

void Client::onReadRecvData()
{
    QByteArray buffer;
    buffer = client_->readAll();
    if (!buffer.isEmpty())
    {
        QString context = ui->client_recvmsg_edit->toPlainText();
        context += tr(buffer);
        ui->client_recvmsg_edit->setText(context);
    }
}

void Client::onRecvServerDisConnected()
{
    ui->client_send_button->setEnabled(false);
    ui->client_sendmsg_edit->setEnabled(false);
    ui->client_recvmsg_edit->setEnabled(false);
}

bool Client::eventFilter(QObject *watched, QEvent *event)
{
    int        type_ = static_cast<int>(event->type());
    QKeyEvent *keys_ = static_cast<QKeyEvent *>(event);

    if (type_ == QEvent::KeyPress && keys_->key() == Qt::Key_Return)
    {
        if (client_->state() == QAbstractSocket::UnconnectedState)
        {
            if (watched == ui->client_port_edit)
            {
                onClientConnectButton();
                return true;
            }
        }
        else if (client_->state() == QAbstractSocket::ConnectedState)
        {
            if (watched == ui->client_sendmsg_edit)
            {
                onClientSendMsgButton();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

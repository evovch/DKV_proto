#include "cls_tcpclient.h"
#include "ui_cls_tcpclient.h"

#include <QDebug>
#include <QList>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTime>

//TODO perform binding
//TODO choose interface

cls_tcpClient::cls_tcpClient(QWidget *parent) :
    QWidget(parent),
    m_nNextBlockSize(0),
    ui(new Ui::cls_tcpClient)
{
    ui->setupUi(this);

    // List all interfaces
    QList<QNetworkInterface> listOfAllInterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface curInterface, listOfAllInterfaces) {
        //qDebug() << curInterface.humanReadableName();
        ui->lstInterfaces->addItem(curInterface.humanReadableName());
    }

    // Select the first interface in the list
    if (ui->lstInterfaces->count() > 0) {
        ui->lstInterfaces->item(0)->setSelected(true);
    }

    // Write default host and port number into the text box
    ui->leHost->setText("127.0.0.1");
    ui->lePort->setText("60000");

    mTcpSocket = new QTcpSocket(this);
    connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)) );
    connect(mTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mTcpSocket, SIGNAL(disconnected()), SLOT(slotDisconnected()));
    connect(mTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

cls_tcpClient::~cls_tcpClient()
{
    delete ui;

    this->DisconnectFromServer();

    delete mTcpSocket;
}

void cls_tcpClient::ConnectToServer()
{
/*
    // Get the selected interface name from the list
    QString interface;
    interface = ui->lstInterfaces->currentItem()->text();

    //TODO implement checks
    if (interface == "") {
    }
*/
    //TODO implement checks
    if (ui->lePort->text() == "") {
        qDebug() << "Incorrect port!";
    }

    unsigned int port = ui->lePort->text().toInt();

    //TODO implement checks
    if (port < 1) {
    }

    // Get the host name from the text box
    QString host = ui->leHost->text();

    //TODO implement checks
    if (host == "") {
        qDebug() << "Incorrect host!";
    }

    qDebug().nospace().noquote() << "Connecting to host " << host << " on port " << port << "...";

    //TODO implement check - if connected

    // Per-se start the client
    mTcpSocket->connectToHost(host, port);
}

void cls_tcpClient::DisconnectFromServer()
{
    //TODO check
    qDebug() << "Disconnecting...";
    mTcpSocket->disconnectFromHost();
}

void cls_tcpClient::SendMessage()
{
    this->SendToServer(ui->lineEdit->text());
}

void cls_tcpClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(mTcpSocket->errorString())
                    );
    qDebug() << strError;
}

void cls_tcpClient::slotConnected()
{
    qDebug() << "Connection established.";

    //TODO check!
    QHostAddress clientHost = mTcpSocket->localAddress();
    quint16 clientPort = mTcpSocket->localPort();
//    QHostAddress serverHost = mTcpSocket->peerAddress();
//    quint16 serverPort = mTcpSocket->peerPort();
//    QString serverName = mTcpSocket->peerName();

    // GUI
    ui->leClientHost->setText(clientHost.toString());
    ui->leClientPort->setText(QString::number(clientPort));
    ui->chkConnected->setChecked(true);
}

void cls_tcpClient::slotDisconnected()
{
    qDebug() << "Disconnected.";

    // GUI
    ui->leClientHost->setText("");
    ui->leClientPort->setText("");
    ui->chkConnected->setChecked(false);
}

void cls_tcpClient::SendToServer(const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    mTcpSocket->write(arrBlock);
}

void cls_tcpClient::slotReadyRead()
{
    QDataStream in(mTcpSocket);
    in.setVersion(QDataStream::Qt_5_8);

    for (;;) {
        if (!m_nNextBlockSize) {
            if (mTcpSocket->bytesAvailable() < sizeof(quint16)) { //TODO fix warning
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (mTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime time;
        QString str;
        in >> time >> str;

        qDebug().nospace().noquote() << time.toString() + " " + str;

        m_nNextBlockSize = 0;
    }
}

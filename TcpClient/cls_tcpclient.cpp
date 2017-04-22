#include "cls_tcpclient.h"
#include "ui_cls_tcpclient.h"

#include <QDebug>
#include <QList>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTime>
#include <QThread>

#include "cls_connstatechecker.h"
#include "commonnetworkconst.h"
#include "support.h"

//TODO perform binding - ?
//TODO choose interface - ?

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
    connect(mTcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(this, SIGNAL(sigDataReceived(QByteArray&)), this, SLOT(slotDataReceived(QByteArray&)));

    // Polling thread - connectivity check
    mPollingThread = new QThread;
    cls_connStateChecker* worker = new cls_connStateChecker; // Correponding delete - ?
    worker->moveToThread(mPollingThread);

    connect(this, SIGNAL(sigStartPoller()), worker, SLOT(Start()));
    connect(this, SIGNAL(sigStopPoller()), worker, SLOT(Stop()));
    connect(worker, SIGNAL(performCheck()), this, SLOT(SingleCheck()));

    connect(worker, SIGNAL(finished()), mPollingThread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(mPollingThread, SIGNAL(finished()), mPollingThread, SLOT(deleteLater()));
    mPollingThread->start();
}

cls_tcpClient::~cls_tcpClient()
{
    delete ui;

    this->on_pbDisconnect_clicked();

    //TODO checks!
    delete mTcpSocket;
    //delete mPollingThread; //TODO Qt manages that?
}

void cls_tcpClient::on_pbConnect_clicked()
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

void cls_tcpClient::on_pbDisconnect_clicked()
{
    //TODO check
    qDebug() << "Disconnecting...";
    mTcpSocket->disconnectFromHost();
}

void cls_tcpClient::on_pbSend_clicked()
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

    // Start polling - connectivity check
    emit sigStartPoller();
}

void cls_tcpClient::slotDisconnected()
{
    qDebug() << "Disconnected.";

    // GUI
    ui->leClientHost->setText("");
    ui->leClientPort->setText("");
    ui->chkConnected->setChecked(false);

    // Stop polling  - connectivity check
    emit sigStopPoller();
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
    QByteArray* buffer = new QByteArray();
    qint32* s = new qint32(0);
    qint32 size = *s;

    while (mTcpSocket->bytesAvailable() > 0)
    {
        buffer->append(mTcpSocket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = Support::ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                emit sigDataReceived(data);
            }
        }
    }

    delete buffer;
    delete s;
}

/*void cls_tcpClient::slotReadyRead()
{
    QDataStream in(mTcpSocket);
    in.setVersion(QDataStream::Qt_5_8);

    for (;;) {
        if (!m_nNextBlockSize) {
            if (mTcpSocket->bytesAvailable() < (qint64)sizeof(quint16)) { //TODO fix warning
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
}*/

void cls_tcpClient::slotDataReceived(QByteArray& data)
{
    qDebug() << "Data received";
    QImage image;
    image.loadFromData(data, "png");
    ui->lblImage->setPixmap(QPixmap::fromImage(image));
}

void cls_tcpClient::SingleCheck()
{
    //this->SendToServer(CHECKREQUEST);
}

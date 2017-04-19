#include "cls_tcpserverscanner.h"

#include <QDebug>
#include <QTimer>

// networking
#include <QTcpSocket>
#include <QHostAddress>

#include "support.h"

cls_tcpServerScanner::cls_tcpServerScanner(QObject *parent) : QObject(parent)
{
    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotBunchTimedOut()));

    // TCP scanning
    for (unsigned int i=0; i<NPARALLELSOCKETS; i++) {
        mTcpSocket[i] = new QTcpSocket(this);
        connect(mTcpSocket[i], SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)) );
        connect(mTcpSocket[i], SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(mTcpSocket[i], SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    }

    goOn = false;
}

//TODO disconnect if any connections are active
cls_tcpServerScanner::~cls_tcpServerScanner()
{
    delete mTimer;

    // TCP scanning
    for (unsigned int i=0; i<NPARALLELSOCKETS; i++) {
        delete mTcpSocket[i];
    }
}

void cls_tcpServerScanner::SetHostRange(QString p_first, QString p_last)
{
    mFirstHost = p_first;
    mLastHost = p_last;
}

void cls_tcpServerScanner::SetPort(unsigned int p_port)
{
    mPort = p_port;
}

void cls_tcpServerScanner::StartScanner()
{
    qDebug() << "cls_tcpServerScanner::StartScanner";

    goOn = true;
    this->NextBunch(mFirstHost);
}

void cls_tcpServerScanner::NextBunch(QString startAddress)
{
    QString host = startAddress;
    QList<QString> curList;

    uint counterBatch = 0;
    bool listReady = false;
    while (!listReady)
    {
        curList.append(host);

        counterBatch++;
        if (counterBatch == NPARALLELSOCKETS) {
            listReady = true;
        }

        if (host == mLastHost) {
            listReady = true;
            goOn = false;
        }
        host = Support::NextAddress(host);
    }

    this->StartBunch(curList);
}

void cls_tcpServerScanner::StartBunch(QList<QString> listOfHosts)
{
    if (listOfHosts.count() > NPARALLELSOCKETS) {
        qDebug() << "Too many hosts in the list!";
        return;
    }

    unsigned int counter = 0;
    foreach (QString item, listOfHosts) {
        this->TryToConnectToServer(mTcpSocket[counter], item, mPort);
        mCurrentHost = item;
        counter++;
    }

    mTimer->start(SCANNERTIMEOUT);
}

void cls_tcpServerScanner::slotBunchTimedOut()
{
    qDebug() << "Bunch timed out!";

    // Stop all connections
    for (unsigned int i=0; i<NPARALLELSOCKETS; i++) {
        qDebug().nospace().noquote() << "state=" << Support::StateToString(mTcpSocket[i]->state()) << ". Aborting.";
        mTcpSocket[i]->abort();
    }

    if (goOn) {
        this->NextBunch(Support::NextAddress(mCurrentHost));
    }
}

void cls_tcpServerScanner::StopScanner()
{
    qDebug() << "cls_tcpServerScanner::StopScanner";
}

void cls_tcpServerScanner::TryToConnectToServer(QTcpSocket* p_tcpSocket, QString p_host, unsigned int p_port)
{
    QHostAddress host(p_host);

    qDebug().nospace().noquote() << "Connecting to host " << host.toString() << " on port " << p_port << "...";

    //TODO implement check - if connected

    // Per-se start the client
    p_tcpSocket->connectToHost(host, p_port);
}

void cls_tcpServerScanner::slotError(QAbstractSocket::SocketError err)
{
    QTcpSocket* pSocket = static_cast<QTcpSocket*>(QObject::sender());

    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(pSocket->errorString())
                    );
    qDebug() << strError;
}

void cls_tcpServerScanner::slotConnected()
{
    qDebug() << "Connection established.";
    QTcpSocket* pSocket = static_cast<QTcpSocket*>(QObject::sender());
    Support::PrintSocketInfo(pSocket);
    emit sigDiscovered(pSocket->peerAddress());
}

void cls_tcpServerScanner::slotDisconnected()
{
    qDebug() << "Disconnected.";
    QTcpSocket* pSocket = static_cast<QTcpSocket*>(QObject::sender());
    Support::PrintSocketInfo(pSocket);
}

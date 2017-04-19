#include "cls_tcpserverscanner.h"

#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>

#include "support.h"

cls_tcpServerScanner::cls_tcpServerScanner(QObject *parent) : QObject(parent)
{
    // TCP scanning
    for (unsigned int i=0; i<NPARALLELSOCKETS; i++) {
        mTcpSocket[i] = new QTcpSocket(this);
        connect(mTcpSocket[i], SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)) );
        connect(mTcpSocket[i], SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(mTcpSocket[i], SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    }
}

//TODO disconnect if any connections are active
cls_tcpServerScanner::~cls_tcpServerScanner()
{
    // TCP scanning
    for (unsigned int i=0; i<NPARALLELSOCKETS; i++) {
        delete mTcpSocket[i];
    }
}

void cls_tcpServerScanner::StartScanner()
{
    qDebug() << "cls_tcpServerScanner::StartScaner";

    QString host = mFirstHost;

    bool stopScan = false;
    uint counterTotal = 0;
    uint counterBatch = 0;
    while (!stopScan)
    {
        QHostAddress curHost(host);
        qDebug() << counterBatch << " " << curHost.toString();
        this->TryToConnectToServer(mTcpSocket[counterBatch], curHost, mPort);

        counterBatch++;
        if (counterBatch == NPARALLELSOCKETS) {
            Support::myqSleep(SCANNERTIMEOUT);

            // Stop all connections
            for (unsigned int i=0; i<NPARALLELSOCKETS; i++) {
                qDebug() << Support::StateToString(mTcpSocket[i]->state()) << ". Aborting.";
                mTcpSocket[i]->abort();
            }

            qDebug() << "----------------------------------------";
            counterBatch = 0;
        }

        if (host == mLastHost) stopScan = true;
        host = Support::NextAddress(host);
        if (counterTotal++ > 1000) break; // limiter on the number of addresses to be scanned
    };

    qDebug() << "Finished scan";
}

void cls_tcpServerScanner::StopScanner()
{
    qDebug() << "cls_tcpServerScanner::StopScanner";
}

void cls_tcpServerScanner::TryToConnectToServer(QTcpSocket* p_tcpSocket, const QHostAddress& p_host, unsigned int p_port)
{
    qDebug().nospace().noquote() << "Connecting to host " << p_host.toString() << " on port " << p_port << "...";

    //TODO implement check - if connected

    // Per-se start the client
    p_tcpSocket->connectToHost(p_host, p_port);
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
}

void cls_tcpServerScanner::slotDisconnected()
{
    qDebug() << "Disconnected.";
    QTcpSocket* pSocket = static_cast<QTcpSocket*>(QObject::sender());
    Support::PrintSocketInfo(pSocket);
}

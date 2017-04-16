#include "cls_broadcastreceiver.h"

#include <QByteArray>
#include <QDebug>

// networking
#include <QUdpSocket>
#include <QNetworkAddressEntry>
#include <QNetworkDatagram>

#include "commonnetworkconst.h"
#include "support.h"

cls_BroadcastReceiver::cls_BroadcastReceiver(QObject *parent) :
    QObject(parent)
{
    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->bind(DISCOVERYSERVERPORT, QUdpSocket::ShareAddress);

    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(ProcessPendingDatagrams()));
}

cls_BroadcastReceiver::~cls_BroadcastReceiver()
{
    //TODO check
    delete mUdpSocket;
}

void cls_BroadcastReceiver::ProcessPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = mUdpSocket->receiveDatagram();
/*
        QList<QNetworkInterface> listOfAllInterfaces = QNetworkInterface::allInterfaces();
        foreach (QNetworkInterface curInterface, listOfAllInterfaces) {
            qDebug() << curInterface.index() << " " << curInterface.humanReadableName();
        }
//        listOfAllInterfaces.
*/

        qDebug() << "Received";
        Support::PrintDatagramInfo(datagram);

        QString prefix(DISCOVERYREQUEST);
        QString strDatagram(datagram.data());
        if (strDatagram.startsWith(prefix)) {
            this->Reply(datagram);
        }

    }
}

void cls_BroadcastReceiver::Reply(QNetworkDatagram& requestDatagram)
{
    uint interfaceIndex = 0;
    QHostAddress clientAddress = requestDatagram.senderAddress();

    QList<QNetworkInterface> listOfAllInterfaces = QNetworkInterface::allInterfaces();
    QList<QNetworkAddressEntry> allEntries = listOfAllInterfaces.at(interfaceIndex).addressEntries();

    QByteArray datagramBA(DISCOVERYANSWER);
    QNetworkDatagram datagram;
    datagram.setData(datagramBA);
    datagram.setDestination(clientAddress, DISCOVERYCLIENTPORT);

    qDebug() << "Sending";
    Support::PrintDatagramInfo(datagram);

    mUdpSocket->writeDatagram(datagram);
}

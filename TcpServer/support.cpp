#include "support.h"

#include <QHostAddress>
#include <QNetworkDatagram>

QString Support::StateToString(QAbstractSocket::SocketState stat)
{
    switch (stat) {
        case QAbstractSocket::UnconnectedState : return "Unconnected"; break;
        case QAbstractSocket::HostLookupState : return "HostLookup"; break;
        case QAbstractSocket::ConnectingState : return "Connecting"; break;
        case QAbstractSocket::ConnectedState : return "Connected"; break;
        case QAbstractSocket::BoundState : return "Bound"; break;
        case QAbstractSocket::ClosingState : return "Closing"; break;
        case QAbstractSocket::ListeningState : return "Listening"; break;
        default: return "Unknown"; break;
    }
}

void Support::PrintDatagramInfo(QNetworkDatagram& datagram)
{
    uint interfaceInd = datagram.interfaceIndex();
    QHostAddress sndrAddr = datagram.senderAddress();
    int sndrPort = datagram.senderPort();
    QHostAddress rcvrAddr = datagram.destinationAddress();
    int rcvrPort = datagram.destinationPort();

    qDebug().nospace().noquote() << "Datagram: interface " << interfaceInd;
    qDebug().nospace().noquote() << "  from " << sndrAddr.toString() << ":" << sndrPort;
    qDebug().nospace().noquote() << "  to " << rcvrAddr.toString() << ":" << rcvrPort;
    qDebug().nospace().noquote() << "  with the following data:";
    qDebug().nospace().noquote() << "  \"" << datagram.data() << "\"";
}

#include "support.h"

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QHostAddress>
#include <QTcpSocket>

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

void Support::PrintSocketInfo(QTcpSocket* socket)
{
    QHostAddress clientHost = socket->localAddress();
    quint16 clientPort = socket->localPort();
    QHostAddress serverHost = socket->peerAddress();
    quint16 serverPort = socket->peerPort();
    QString serverName = socket->peerName();

    qDebug().nospace().noquote() << "Socket: client " << clientHost << ":" << clientPort;
    qDebug().nospace().noquote() << "  server " << serverHost << ":" << serverPort << " name=" << serverName;
}

QString Support::NextAddress(QString curAddress)
{
    QHostAddress cur(curAddress);
    quint32 curNum = cur.toIPv4Address();

    QHostAddress next;

    //TODO check!
    if ((curNum & 0xff) == 0xfe ) {
        curNum += 3;
    } else {
        curNum += 1;
    }
    next.setAddress(curNum);
    return next.toString();
}

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
void Support::myqSleep(int ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

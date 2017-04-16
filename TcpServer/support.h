#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <QString>
#include <QAbstractSocket>

class QNetworkDatagram; //TODO enough?

class Support
{
public:
    static QString StateToString(QAbstractSocket::SocketState stat);
    static void PrintDatagramInfo(QNetworkDatagram& datagram);
};

#endif // SUPPORT_HPP

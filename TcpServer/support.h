#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <QString>
#include <QAbstractSocket>

class QNetworkDatagram; //TODO enough?
class QTcpSocket;

class Support
{
public:
    static QString StateToString(QAbstractSocket::SocketState stat);
    static void PrintDatagramInfo(QNetworkDatagram& datagram);
    static void PrintSocketInfo(QTcpSocket* socket);
    static QString NextAddress(QString curAddress);
    /*static void myqSleep(int ms);*/

};

#endif // SUPPORT_HPP

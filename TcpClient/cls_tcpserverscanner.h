#ifndef CLS_TCPSERVERSCANNER_H
#define CLS_TCPSERVERSCANNER_H

#include <QObject>
#include <QAbstractSocket>

#define NPARALLELSOCKETS 10
#define SCANNERTIMEOUT 2000

class QTcpSocket;
class QHostAddress; //TODO enough?

class cls_tcpServerScanner : public QObject
{
    Q_OBJECT
public:
    explicit cls_tcpServerScanner(QObject *parent = 0);
    ~cls_tcpServerScanner();

    void SetHostRange(QString p_first, QString p_last) { mFirstHost = p_first; mLastHost = p_last; }
    void SetPort(unsigned int p_port) { mPort = p_port; }

signals:

public slots:
    void StartScanner();
    void StopScanner();

    /*
     * These slots are connected to some signals in the code.
     * TCP scanner.
     */
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotDisconnected();

private:
    // TCP scanner.
    void TryToConnectToServer(QTcpSocket* p_tcpSocket, const QHostAddress& p_host, unsigned int p_port);
    QTcpSocket* mTcpSocket[NPARALLELSOCKETS];

    unsigned int mPort;

    QString mFirstHost;
    QString mLastHost;

};

#endif // CLS_TCPSERVERSCANNER_H

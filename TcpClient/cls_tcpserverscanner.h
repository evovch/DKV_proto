#ifndef CLS_TCPSERVERSCANNER_H
#define CLS_TCPSERVERSCANNER_H

#include <QObject>
#include <QList>
#include <QString>

// networking
#include <QAbstractSocket>
#include <QHostAddress>

#define NPARALLELSOCKETS 10
#define SCANNERTIMEOUT 2000

class QTcpSocket;
class QTimer;

class cls_tcpServerScanner : public QObject
{
    Q_OBJECT
public:
    explicit cls_tcpServerScanner(QObject *parent = 0);
    ~cls_tcpServerScanner();

    void SetHostRange(QString p_first, QString p_last);
    void SetPort(unsigned int p_port);

signals:
    void sigDiscovered(QHostAddress discoveredHost);
    void sigScanFinished();

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

    void slotBunchTimedOut();

private:
    // TCP scanner.
    void TryToConnectToServer(QTcpSocket* p_tcpSocket, QString p_host, unsigned int p_port);
    QTcpSocket* mTcpSocket[NPARALLELSOCKETS];

    unsigned int mPort;

    QString mFirstHost;
    QString mCurrentHost;
    QString mLastHost;
    bool goOn;

    void NextBunch(QString startAddress);
    void StartBunch(QList<QString> listOfHosts);
    QTimer* mTimer;

};

#endif // CLS_TCPSERVERSCANNER_H

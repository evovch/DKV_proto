#ifndef CLS_SERVERFINDER_H
#define CLS_SERVERFINDER_H

#include <QWidget>

namespace Ui {
    class cls_ServerFinder;
}

class QTcpSocket;
class QHostAddress; //TODO enough?

class QUdpSocket;

class cls_ServerFinder : public QWidget
{
    Q_OBJECT

public:
    explicit cls_ServerFinder(QWidget *parent = 0);
    ~cls_ServerFinder();

public slots:
    void StartScan();
    void SendBroadcast();
    void ProcessPendingDatagrams();

private:
    void TryToConnectToServer(const QHostAddress& p_host, unsigned int p_port);
    QTcpSocket* mTcpSocket;
    quint16 m_nNextBlockSize;

private:
    Ui::cls_ServerFinder *ui;

    QUdpSocket* mUdpSndSocket; // Socket to send the broadcast requests
    QUdpSocket* mUdpRecvSocket; // Socket to receive the answer from the discovered servers

};

#endif // CLS_SERVERFINDER_H

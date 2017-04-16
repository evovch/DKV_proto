#ifndef CLS_BROADCASTRECEIVER_H
#define CLS_BROADCASTRECEIVER_H

#include <QObject>

class QNetworkDatagram; //TODO enough?
class QUdpSocket;

class cls_BroadcastReceiver : public QObject
{
    Q_OBJECT

public:
    explicit cls_BroadcastReceiver(QObject *parent = 0);
    ~cls_BroadcastReceiver();

public slots:
    void ProcessPendingDatagrams();
    void Reply(QNetworkDatagram& requestDatagram);

private:
    QUdpSocket* mUdpSocket; // Socket to receive the broadcast requests

};

#endif // CLS_BROADCASTRECEIVER_H

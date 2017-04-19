#ifndef CLS_SERVERFINDER_H
#define CLS_SERVERFINDER_H

#include <QWidget>

namespace Ui {
    class cls_ServerFinder;
}

class QUdpSocket;
class QThread;
class cls_tcpServerScanner;

class cls_serverFinder : public QWidget
{
    Q_OBJECT

public:
    explicit cls_serverFinder(QWidget *parent = 0);
    ~cls_serverFinder();

public slots:
    /*
     * These slots are connected to the GUI objects in the Designer.
     */
    void StartScan();
    void SendBroadcast();

    /*
     * These slots are connected to some signals in the code.
     * UDP broadcast.
     */
    void ProcessPendingDatagrams();

signals:
    void sigStartScanner();
    void sigStopScanner();

private:
    Ui::cls_ServerFinder *ui;

    // TCP scanner.
    QThread* tcpScannerThread;
    cls_tcpServerScanner* workerScanner;

    // UDP broadcast.
    QUdpSocket* mUdpSndSocket; // Socket to send the broadcast requests
    QUdpSocket* mUdpRecvSocket; // Socket to receive the answer from the discovered servers

};

#endif // CLS_SERVERFINDER_H

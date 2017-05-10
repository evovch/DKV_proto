#ifndef CLS_SERVERFINDER_H
#define CLS_SERVERFINDER_H

#include <QWidget>

#include <QHostAddress>

namespace Ui {
    class cls_serverFinder;
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
     * These slots are connected to some signals in the code.
     * UDP broadcast.
     */
    void ProcessPendingDatagrams();

    /*
     * TCP scanner.
     */
    void AddDiscoveredServer(QHostAddress discoveredHost);
    void ScanFinished();

signals:
    void sigStartScanner();
    void sigStopScanner();

private slots:
    void on_pbStartScan_clicked();
    void on_pbStopScan_clicked();
    void on_pbSendBroadcast_clicked();

private:
    Ui::cls_serverFinder *ui;

    // TCP scanner.
    QThread* tcpScannerThread;
    cls_tcpServerScanner* workerScanner;

    // UDP broadcast.
    QUdpSocket* mUdpSndSocket; // Socket to send the broadcast requests
    QUdpSocket* mUdpRecvSocket; // Socket to receive the answer from the discovered servers

};

#endif // CLS_SERVERFINDER_H

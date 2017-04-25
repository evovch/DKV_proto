#ifndef CLS_TCPCLIENT_H
#define CLS_TCPCLIENT_H

#include <QWidget>
#include <QAbstractSocket>

namespace Ui {
    class cls_tcpClient;
}

class QTcpSocket;
class QThread;

class cls_tcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit cls_tcpClient(QWidget *parent = 0);
    ~cls_tcpClient();

    /*
     * Send a string to the server.
     */
    void SendToServer(const QString& str);

public slots:
    /*
     * These slots are connected to some signals in the code.
     */
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();

    /*
     * This slot is called from the connection checker.
     */
    void SingleCheck();

signals:
    void sigStartPoller();
    void sigStopPoller();
    void sigDataReceived(QByteArray& data);

public slots:
    void on_pbConnect_clicked();
    void on_pbDisconnect_clicked();
    void on_pbSend_clicked();

    void slotDataReceived(QByteArray& data);

private:
    QTcpSocket* mTcpSocket;

    quint16 m_nNextBlockSize;

    QThread* mPollingThread;

    // Image receiving
    QByteArray* mBuffer;
    qint32 mSize;
    bool mWaitForSize;

private:
    Ui::cls_tcpClient *ui;

};

#endif // CLS_TCPCLIENT_H

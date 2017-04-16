#ifndef CLS_TCPCLIENT_H
#define CLS_TCPCLIENT_H

#include <QWidget>
#include <QAbstractSocket>

namespace Ui {
    class cls_tcpClient;
}

class QTcpSocket;

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
     * These slots are connected to the GUI objects in the Designer.
     */
    void ConnectToServer();
    void DisconnectFromServer();
    void SendMessage();

    /*
     * These slots are connected to some signals in the code.
     */
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();

private:
    QTcpSocket* mTcpSocket;

    quint16 m_nNextBlockSize;

private:
    Ui::cls_tcpClient *ui;

};

#endif // CLS_TCPCLIENT_H

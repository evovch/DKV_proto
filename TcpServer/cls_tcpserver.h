#ifndef CLS_TCPSERVER_H
#define CLS_TCPSERVER_H

#include <QWidget>
#include <QMap>
#include <QTcpSocket>

namespace Ui {
    class cls_tcpServer;
}

class QTcpServer;

class cls_tcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit cls_tcpServer(QWidget *parent = 0);
    ~cls_tcpServer();

    /*
     * Perform all possible checks of the server state.
     * In particular - check the state of all the sockets.
     * Return true is everything is fine and false if any kind of error is found.
     */
    bool IsHealthy();

    /*
     * Send a string to a certain client identified using the socket-argument.
     */
    void sendToClient(QTcpSocket* pSocket, const QString& str);

public slots:
    /*
     * These slots are connected to the GUI objects in the Designer.
     */
    void StartServer();
    void StopServer();
    void DisconnectClient();

    /*
     * These slots are connected to some signals in the code.
     */
    void slotError(QAbstractSocket::SocketError);
    virtual void slotNewConnection();
    void slotClientDisconnect();
    void slotUpdateSocketsState();
    void slotReadClient();

private:
    QTcpServer* mTcpServer;
    QMap<QString, QTcpSocket*> mListOfSockets;

    quint16 m_nNextBlockSize;

    void React(QTcpSocket* p_socket, const QString& p_receivedMessage);

private:
    Ui::cls_tcpServer *ui;

};

#endif // CLS_TCPSERVER_H

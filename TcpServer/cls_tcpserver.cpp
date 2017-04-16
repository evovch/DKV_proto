#include "cls_tcpserver.h"
#include "ui_cls_tcpserver.h"

#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

#include "support.h"

cls_tcpServer::cls_tcpServer(QWidget *parent) :
    QWidget(parent),
    m_nNextBlockSize(0),
    ui(new Ui::cls_tcpServer)
{
    ui->setupUi(this);

    // List all interfaces
    QList<QNetworkInterface> listOfAllInterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface curInterface, listOfAllInterfaces) {
        //qDebug() << curInterface.humanReadableName();
        ui->lstInterfaces->addItem("(" + QString::number(curInterface.index()) + ") " + curInterface.humanReadableName());
    }

    // Select the first interface in the list
    if (ui->lstInterfaces->count() > 0) {
        ui->lstInterfaces->item(0)->setSelected(true);
    }

    // Write default port number into the text box
    ui->lePort->setText("60000");

    // Create the TCP server
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    // GUI
    ui->tableWidget->setColumnCount(2);
}

cls_tcpServer::~cls_tcpServer()
{
    delete ui;
}

void cls_tcpServer::StartServer()
{
/*
    // Get the selected interface name from the list
    QString interface;
    interface = ui->lstInterfaces->currentItem()->text();

    //TODO implement checks
    if (interface == "") {
    }
*/
    //TODO implement checks
    if (ui->lePort->text() == "") {
        qDebug() << "Incorrect port!";
        return;
    }

    unsigned int port = ui->lePort->text().toInt();

    //TODO implement checks
    if (port < 1) {
    }

    qDebug().nospace().noquote() << "Starting server on port " << port << "...";

    //TODO In principle it is possible to start two or more servers on the same port of
    // one machine and both will run the 'listen' method successfully.
    // However only one server will actually receive connection.


    // Per-se start the server
    if (!mTcpServer->listen(QHostAddress::Any, port)) {
        //mTcpServer->close();
        return;
    }

    // GUI
    ui->checkBox->setChecked(true);
}

void cls_tcpServer::StopServer()
{
    qDebug().nospace().noquote() << "Stopping server...";

    //TODO send signal to all connected clients - seems to be done automatically by Qt

    foreach (QTcpSocket* curSocket, mListOfSockets) {
        curSocket->close(); // We need to do this manually!
    }

    mTcpServer->close();

    // GUI
    ui->checkBox->setChecked(false);
}

void cls_tcpServer::DisconnectClient()
{
    QList<QListWidgetItem*> selectedSublist = ui->lstSockets->selectedItems();

    foreach (QListWidgetItem* curSelectedItem, selectedSublist) {
        QTcpSocket* curSocket = *(mListOfSockets.find(curSelectedItem->text()));
        curSocket->close();
    }
}

void cls_tcpServer::slotError(QAbstractSocket::SocketError err)
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(QObject::sender());

    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(pClientSocket->errorString())
                    );
    qDebug() << strError;
}

/*virtual*/ void cls_tcpServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = mTcpServer->nextPendingConnection();

    if (pClientSocket == nullptr) {
        qDebug() << "nextPendingConnection() returned nullptr";
        return;
    }

    //TODO study!
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()) );

    connect(pClientSocket, SIGNAL(disconnected()),
            this, SLOT(slotClientDisconnect()) );

    connect(pClientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(slotUpdateSocketsState()) );

    connect(pClientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)) );

    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()) );

    //QHostAddress serverHost = pClientSocket->localAddress();
    //quint16 serverPort = pClientSocket->localPort();
    QHostAddress clientHost = pClientSocket->peerAddress();
    quint16 clientPort = pClientSocket->peerPort();
    //QString clientName = pClientSocket->peerName();
    QString clientID = clientHost.toString() + ":" + QString::number(clientPort);

    // GUI Table representation
    //FIXME possible memory leakage?
    int curRowCount = ui->tableWidget->rowCount();
    QTableWidgetItem* curItem1 = new QTableWidgetItem(clientID);
    QTableWidgetItem* curItem2 = new QTableWidgetItem(Support::StateToString(pClientSocket->state()));
    ui->tableWidget->insertRow(curRowCount);
    ui->tableWidget->setItem(curRowCount, 0, curItem1);
    ui->tableWidget->setItem(curRowCount, 1, curItem2);

    // GUI List representation
    ui->lstSockets->addItem(clientID);

    // Fill the map of the sockets
    mListOfSockets.insert(clientID, pClientSocket);

    qDebug().nospace().noquote() << "Client " << clientID << " connected.";
}

// This slot is activated when the client disconnects from the server
void cls_tcpServer::slotClientDisconnect()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(QObject::sender());

    //QHostAddress serverHost = pClientSocket->localAddress();
    //quint16 serverPort = pClientSocket->localPort();
    QHostAddress clientHost = pClientSocket->peerAddress();
    quint16 clientPort = pClientSocket->peerPort();
    //QString clientName = pClientSocket->peerName();
    QString clientID = clientHost.toString() + ":" + QString::number(clientPort);

    // Remove from the map of the sockets
    //TODO checks
    mListOfSockets.remove(clientID);

    // GUI List representation
    //FIXME possible memory leakage due to takeItem
    for (int i=0; i<ui->lstSockets->count(); i++) {
        if (ui->lstSockets->item(i)->text() == clientID) {
            ui->lstSockets->takeItem(i);
            break;
        }
    }

    qDebug().nospace().noquote() << "Client " << clientID << " disconnected.";
}

// This slot is activated when the state of any socket changes.
// Here we indicate the current connection state in the GUI.
void cls_tcpServer::slotUpdateSocketsState()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(QObject::sender());

    //QHostAddress serverHost = pClientSocket->localAddress();
    //quint16 serverPort = pClientSocket->localPort();
    QHostAddress clientHost = pClientSocket->peerAddress();
    quint16 clientPort = pClientSocket->peerPort();
    //QString clientName = pClientSocket->peerName();
    QString clientID = clientHost.toString() + ":" + QString::number(clientPort);

    for (int i=0; i<ui->tableWidget->rowCount(); i++) {
        QTableWidgetItem* curItem1 = ui->tableWidget->item(i, 0);
        QTableWidgetItem* curItem2 = ui->tableWidget->item(i, 1);
        if (curItem1->text() == clientID) {
            curItem2->setText(Support::StateToString(pClientSocket->state()));
            //break; //TODO test
        }
    }

    //qDebug().nospace().noquote() << "State of the connection with client " << clientID << " has changed to "
    //                             << Support::StateToString(pClientSocket->state());
}

bool cls_tcpServer::IsHealthy()
{
    //TODO implement?
    return true;
}

void cls_tcpServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}

void cls_tcpServer::slotReadClient()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(QObject::sender());

    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_8);

    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) { //TODO fix warning
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime time;
        QString str;
        in >> time >> str;

        QString strMessage = time.toString() + " " + "Client has sended \"" + str + "\"";
        qDebug().nospace().noquote() << strMessage;

        m_nNextBlockSize = 0;

        this->React(pClientSocket, str);
    }
}

void cls_tcpServer::React(QTcpSocket* p_socket, const QString& p_receivedMessage)
{
    if (p_receivedMessage == "AreYouCamera?") {
        sendToClient(p_socket, "Yes, I am camera!");
    } else {
        sendToClient(p_socket, "Server Response: Received \"" + p_receivedMessage + "\"" );
    }
}

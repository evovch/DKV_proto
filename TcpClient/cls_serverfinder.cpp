#include "cls_serverfinder.h"
#include "ui_cls_serverfinder.h"

#include <QList>

// networking
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QUdpSocket>

#include "commonnetworkconst.h"
#include "support.h"

cls_ServerFinder::cls_ServerFinder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_ServerFinder)
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

    ui->leFirstIP->setText("192.168.0.1");
    ui->leLastIP->setText("192.168.0.254");

    mTcpSocket = new QTcpSocket(this);

    // Broadcasting: request sending socket
    mUdpSndSocket = new QUdpSocket(this);

    // Broadcasting: answer reciving socket
    mUdpRecvSocket = new QUdpSocket(this);
    mUdpRecvSocket->bind(DISCOVERYCLIENTPORT, QUdpSocket::ShareAddress);

    connect(mUdpRecvSocket, SIGNAL(readyRead()), this, SLOT(ProcessPendingDatagrams()));
}

cls_ServerFinder::~cls_ServerFinder()
{
    delete ui;

    //TODO disconnect if any connections are active

    //delete mTcpSocket;

    //TODO check
    delete mUdpSndSocket;
    delete mUdpRecvSocket;
}

void cls_ServerFinder::StartScan()
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
    }

    unsigned int port = ui->lePort->text().toInt();

    //TODO implement checks
    if (port < 1) {
    }
/*
    // Get the host name from the text box
    QString host = ui->leFirstIP->text();

    //TODO implement checks
    if (host == "") {
        qDebug() << "Incorrect host!";
    }

    QHostAddress curHost(host);

    this->TryToConnectToServer(curHost, port);
*/
}

void cls_ServerFinder::TryToConnectToServer(const QHostAddress& p_host, unsigned int p_port)
{
    qDebug().nospace().noquote() << "Connecting to host " << p_host.toString() << " on port " << p_port << "...";

    //TODO implement check - if connected

    // Per-se start the client
    mTcpSocket->connectToHost(p_host, p_port);
}

void cls_ServerFinder::SendBroadcast()
{
/*
    // Get the selected interface name from the list
    QString interface;
    interface = ui->lstInterfaces->currentItem()->text();

    //TODO implement checks
    if (interface == "") {
        qDebug() << "Incorrect interface!";
        return;
    }
*/
/*
    //FIXME very stupid string comparison by now
    QNetworkInterface selectedInterface;
    QList<QNetworkInterface> listOfAllInterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface curInterface, listOfAllInterfaces) {
        if (curInterface.humanReadableName() == interface) {
            selectedInterface = curInterface;
            break;
        }
    }
*/
/*
    QNetworkInterface selectedInterface;
    QList<QNetworkInterface> listOfAllInterfaces = QNetworkInterface::allInterfaces();
    selectedInterface = listOfAllInterfaces.at(ui->lstInterfaces->currentRow());

    QList<QNetworkAddressEntry> allEntries = selectedInterface.addressEntries();
    QNetworkAddressEntry entry;
    foreach (entry, allEntries) {
        //qDebug() << entry.ip().toString() << "/" << entry.netmask().toString();
    }

    // Send the broadcast UDP datagram

    QString localIP = allEntries.at(0).ip().toString(); //FIXME - here I select the first address, this may be wrong
    QByteArray localIPba = localIP.toLatin1();
*/
    QByteArray datagramBA = DISCOVERYREQUEST;// + localIPba;

    QNetworkDatagram datagram;
    datagram.setData(datagramBA);
    //datagram.setInterfaceIndex();
    datagram.setDestination(QHostAddress::Broadcast, DISCOVERYSERVERPORT);

    qDebug() << "Sending";
    Support::PrintDatagramInfo(datagram);

    mUdpSndSocket->writeDatagram(datagram);
}

void cls_ServerFinder::ProcessPendingDatagrams()
{
    while (mUdpRecvSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = mUdpRecvSocket->receiveDatagram();
        qDebug() << "Received";
        Support::PrintDatagramInfo(datagram);
/*
        QString prefix(DISCOVERYANSWER);
        QString strDatagram(datagram);
        if (strDatagram.startsWith(prefix)) {
            QString serverAddress = strDatagram.mid(prefix.length());
            qDebug() << serverAddress;
        }
*/
    }
}

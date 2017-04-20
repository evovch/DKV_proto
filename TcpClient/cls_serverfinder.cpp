#include "cls_serverfinder.h"
#include "ui_cls_serverfinder.h"

#include <QList>
#include <QThread>

// networking
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QUdpSocket>

#include "commonnetworkconst.h"
#include "support.h"
#include "cls_tcpserverscanner.h"

cls_serverFinder::cls_serverFinder(QWidget *parent) :
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

    //TODO - temporary
    ui->lstInterfaces->setEnabled(false);

    // Write default port number into the text box
    ui->lePort->setText("60000");

    ui->leFirstIP->setText("192.168.0.1");
    ui->leLastIP->setText("192.168.0.254");

    // TCP scanner.
    tcpScannerThread = new QThread;
    workerScanner = new cls_tcpServerScanner; // Correponding delete - ?
    workerScanner->moveToThread(tcpScannerThread);
    connect(this, SIGNAL(sigStartScanner()), workerScanner, SLOT(StartScanner()));
    connect(this, SIGNAL(sigStopScanner()), workerScanner, SLOT(StopScanner()));
    qRegisterMetaType<QHostAddress>("QHostAddress");
    connect(workerScanner, SIGNAL(sigDiscovered(QHostAddress)), this, SLOT(AddDiscoveredServer(QHostAddress)));
    connect(workerScanner, SIGNAL(sigScanFinished()), this, SLOT(ScanFinished()));
    tcpScannerThread->start();

    // Broadcasting: request sending socket
    mUdpSndSocket = new QUdpSocket(this);

    // Broadcasting: answer receiving socket
    mUdpRecvSocket = new QUdpSocket(this);
    mUdpRecvSocket->bind(DISCOVERYCLIENTPORT, QUdpSocket::ShareAddress);

    connect(mUdpRecvSocket, SIGNAL(readyRead()), this, SLOT(ProcessPendingDatagrams()));

    //GUI
    ui->tableWidget->setColumnCount(1);
    ui->pbStopScan->setEnabled(false);
}

cls_serverFinder::~cls_serverFinder()
{
    delete ui;

    //TODO stop the TCP scanner somehow?

    //TODO check
    delete mUdpSndSocket;
    delete mUdpRecvSocket;
}

void cls_serverFinder::on_pbStartScan_clicked()
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

        // Get the host name from the text box
        QString host = ui->leFirstIP->text();

        //TODO implement checks
        if (host == "") {
            qDebug() << "Incorrect host!";
        }

        // GUI
        // First clean the table of previously discovered servers
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);

        ui->pbStartScan->setEnabled(false);
        ui->pbStopScan->setEnabled(true);

        workerScanner->SetPort(port);
        workerScanner->SetHostRange(ui->leFirstIP->text(), ui->leLastIP->text());
        emit sigStartScanner();
}

void cls_serverFinder::AddDiscoveredServer(QHostAddress discoveredHost)
{
    //qDebug() << "AddDiscoveredServer";

    // GUI
    int curRowCount = ui->tableWidget->rowCount();
    QTableWidgetItem* curItem = new QTableWidgetItem(discoveredHost.toString());
    ui->tableWidget->insertRow(curRowCount);
    ui->tableWidget->setItem(curRowCount, 0, curItem);
}

void cls_serverFinder::ScanFinished()
{
    qDebug() << "Scan finished";
    ui->pbStartScan->setEnabled(true);
    ui->pbStopScan->setEnabled(false);
}

void cls_serverFinder::on_pbStopScan_clicked()
{
    emit sigStopScanner();
}

// UDP broadcast part =================================================================================================

// UDP broadcast request sender
void cls_serverFinder::on_pbSendBroadcast_clicked()
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

        // GUI
        // First clean the table of previously discovered servers
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);

        QByteArray datagramBA = DISCOVERYREQUEST;// + localIPba;

        QNetworkDatagram datagram;
        datagram.setData(datagramBA);
        //datagram.setInterfaceIndex();
        datagram.setDestination(QHostAddress::Broadcast, DISCOVERYSERVERPORT);

        qDebug() << "Sending";
        Support::PrintDatagramInfo(datagram);

        mUdpSndSocket->writeDatagram(datagram);
}

// UDP broadcast response catcher
void cls_serverFinder::ProcessPendingDatagrams()
{
    while (mUdpRecvSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = mUdpRecvSocket->receiveDatagram();
        qDebug() << "Received";
        Support::PrintDatagramInfo(datagram);

        QString prefix(DISCOVERYANSWER);
        QString strDatagram(datagram.data());
        if (strDatagram.startsWith(prefix)) {
            QHostAddress serverAddress = datagram.senderAddress();
            QString strServerAddress = serverAddress.toString();

            // GUI
            int curRowCount = ui->tableWidget->rowCount();
            QTableWidgetItem* curItem = new QTableWidgetItem(strServerAddress);
            ui->tableWidget->insertRow(curRowCount);
            ui->tableWidget->setItem(curRowCount, 0, curItem);
        }
    }
}

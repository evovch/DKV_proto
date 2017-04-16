#include "cls_tcpserver.h"

#include <QApplication>
#include <QThread>

#include "cls_broadcastreceiver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cls_tcpServer w;
    w.show();

    // Create and send to a separate thread the broadcast receiver
    // which is used for the device discovery
    //TODO stop the thread?
    cls_BroadcastReceiver* recv = new cls_BroadcastReceiver;
    QThread* receiverThread = new QThread;
    recv->moveToThread(receiverThread);
    receiverThread->start();

    return a.exec();
}

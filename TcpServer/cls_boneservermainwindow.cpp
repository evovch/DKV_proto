#include "cls_boneservermainwindow.h"
#include "ui_cls_boneservermainwindow.h"

#include <QDebug>
#include <QThread>

#include "cls_broadcastreceiver.h"
#include "cls_lvloop.h"
#include "cls_tcpserver.h"
#include "cls_bonecamera.h"

// Singleton management
cls_boneServerMainWindow* cls_boneServerMainWindow::mInstance = nullptr;

cls_boneServerMainWindow::cls_boneServerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cls_boneServerMainWindow)
{
    ui->setupUi(this);

    // Singleton management
    if (mInstance == nullptr) {
        mInstance = this;
    } else {
        qDebug() << "Only one instance of cls_boneServerMainWindow can exist!";
        return;
    }

    // Create and send to a separate thread the broadcast receiver
    // which is used for the device discovery
    //TODO stop the thread?
    cls_BroadcastReceiver* recv = new cls_BroadcastReceiver;
    QThread* receiverThread = new QThread;
    recv->moveToThread(receiverThread);
    receiverThread->start();

    // Create and send to a separate thread the "Live View Loop"
    //TODO stop the thread?
    cls_lvLoop* lvLoopObj = new cls_lvLoop;
    QThread* lvLoopThread = new QThread;
    lvLoopObj->moveToThread(lvLoopThread);
    connect(ui->pbStartLvLoop, SIGNAL(clicked()), lvLoopObj, SLOT(StartLoop()));
    connect(ui->pbStopLvLoop, SIGNAL(clicked()), lvLoopObj, SLOT(StopLoop()));
    lvLoopThread->start();

    // Show the main server sub-window
    cls_tcpServer* theServer = new cls_tcpServer(this);
    ui->mdiArea->addSubWindow(theServer);
    theServer->show();

    mCameraObj = new cls_boneCamera(this);
    mCameraObj->SetDrawWidget(ui->label);
    connect(mCameraObj, SIGNAL(sigFrameReady(const QImage&)), theServer, SLOT(slotSendImageToAll(const QImage&)));
}

cls_boneServerMainWindow::~cls_boneServerMainWindow()
{
    delete ui;

    // Singleton management
    mInstance = nullptr;

    //TODO check
    delete mCameraObj;
}

// Singleton management
cls_boneServerMainWindow* cls_boneServerMainWindow::GetInstance()
{
    if (mInstance == nullptr) {
        return new cls_boneServerMainWindow();
    } else {
        return mInstance;
    }
}

void cls_boneServerMainWindow::on_pushButton_clicked()
{
    mCameraObj->ProduceTestFrame();
}

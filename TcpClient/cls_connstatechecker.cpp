#include "cls_connstatechecker.h"
#include "ui_cls_connstatechecker.h"

#include <QDebug>
#include <QTimer>

cls_connStateChecker::cls_connStateChecker(QObject /*QWidget*/ *parent) :
    QObject(parent) /*QWidget(parent),*/
    /*ui(new Ui::cls_connStateChecker)*/
{
    //ui->setupUi(this);

    mStarted = false;
    mTimer = nullptr;
}

cls_connStateChecker::~cls_connStateChecker()
{
    //delete ui;
}

void cls_connStateChecker::Start()
{
    if (mStarted) { return; }

    qDebug() << "Starting loop...";

    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(SingleCheck()));
    mTimer->start(2*1000);

    mStarted = true;
}

void cls_connStateChecker::Stop()
{
    if (!mStarted) { return; }

    qDebug() << "Stopping loop...";

    disconnect(mTimer, SIGNAL(timeout()), this, SLOT(SingleCheck()));
    delete mTimer; mTimer = nullptr;

    mStarted = false;
}

void cls_connStateChecker::SingleCheck()
{
    if (!mTimer->isActive()) { return; }
    qDebug() << "CHECK!";
    emit performCheck();
}

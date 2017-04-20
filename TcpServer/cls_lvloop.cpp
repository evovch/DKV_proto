#include "cls_lvloop.h"

#include <QDebug>
#include <QTimer>

#define LVLOOPTIMERPERIOD 5000 // ms

cls_lvLoop::cls_lvLoop(QObject *parent) :
    QObject(parent)
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(OnTimer()), Qt::DirectConnection);

    mRunning = false;
}

cls_lvLoop::~cls_lvLoop()
{
    //TODO check
    delete mTimer;
}

void cls_lvLoop::StartLoop()
{
    if (mRunning) return; // Already running
    mTimer->start(LVLOOPTIMERPERIOD);
    mRunning = true;
    emit sigLvLoopStarted();
}

void cls_lvLoop::StopLoop()
{
    if (!mRunning) return; // Already stopped
    mTimer->stop();
    mRunning = false;
    emit sigLvLoopStopped();
}

void cls_lvLoop::OnTimer()
{
    if (!mRunning) return; // Already stopped (This should never happen!)
    qDebug() << "LvLoop timer";
    emit sigLvLoopTimer();
}

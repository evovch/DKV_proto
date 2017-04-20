#ifndef CLS_LVLOOP_H
#define CLS_LVLOOP_H

#include <QObject>

class QTimer;

class cls_lvLoop : public QObject
{
    Q_OBJECT

public:
    explicit cls_lvLoop(QObject *parent = 0);
    ~cls_lvLoop();

signals:
    void sigLvLoopStarted();
    void sigLvLoopStopped();
    void sigLvLoopTimer();

public slots:
    void StartLoop();
    void StopLoop();
    void OnTimer();

private:
    QTimer* mTimer;

    bool mRunning;

};

#endif // CLS_LVLOOP_H

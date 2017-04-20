#ifndef CLS_BONECAMERA_H
#define CLS_BONECAMERA_H

#include <QObject>

class cls_boneCamera : public QObject
{
    Q_OBJECT
public:
    explicit cls_boneCamera(QObject *parent = 0);
    ~cls_boneCamera();

signals:
    void sigFrameReady();

public slots:

};

#endif // CLS_BONECAMERA_H

#ifndef CLS_BONECAMERA_H
#define CLS_BONECAMERA_H

#include <QObject>

class QLabel;

class cls_boneCamera : public QObject
{
    Q_OBJECT
public:
    explicit cls_boneCamera(QObject *parent = 0);
    ~cls_boneCamera();

    void SetDrawWidget(QLabel* p_widget) { mDrawWidget = p_widget; }
    void ProduceTestFrame();

signals:
    void sigFrameReady(const QImage&);

public slots:

private:
    QLabel* mDrawWidget;

};

#endif // CLS_BONECAMERA_H

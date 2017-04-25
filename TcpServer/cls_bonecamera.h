#ifndef CLS_BONECAMERA_H
#define CLS_BONECAMERA_H

#include <QObject>

//class QLabel;
class QGraphicsView;

class cls_boneCamera : public QObject
{
    Q_OBJECT
public:
    explicit cls_boneCamera(QObject *parent = 0);
    ~cls_boneCamera();

    //void SetDrawWidget(QLabel* p_widget) { mDrawWidget = p_widget; }
    void setGrWidget(QGraphicsView* p_grView) { mGrView = p_grView; }

signals:
    void sigFrameReady(const QImage&);

public slots:
    void ProduceTestFrame();

private:
    //QLabel* mDrawWidget;
    QGraphicsView* mGrView;

    static unsigned int mFrameCounter;

};

#endif // CLS_BONECAMERA_H

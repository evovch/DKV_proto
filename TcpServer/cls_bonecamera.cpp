#include "cls_bonecamera.h"

#include <QString>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QLabel>

cls_boneCamera::cls_boneCamera(QObject *parent) : QObject(parent)
{
}

cls_boneCamera::~cls_boneCamera()
{
}

void cls_boneCamera::ProduceTestFrame()
{
    QString filename1("/home/evovch/Downloads/TestFrame2.png");

    QImage image1;
    image1.load(filename1, "png");

    mDrawWidget->setPixmap(QPixmap::fromImage(image1));

    emit sigFrameReady(image1);
}

#include "cls_bonecamera.h"

#include <QDebug>
#include <QString>
#include <QImage>
#include <QPixmap>
//#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

unsigned int cls_boneCamera::mFrameCounter = 0;

cls_boneCamera::cls_boneCamera(QObject *parent) : QObject(parent)
{
}

cls_boneCamera::~cls_boneCamera()
{
}

void cls_boneCamera::ProduceTestFrame()
{
    QString folderName("/home/evovch/Downloads/motion_jpeg_2/");

    QString filename = folderName;
    filename += "DSC_00" + QString::number(32+mFrameCounter) + ".JPG";
    mFrameCounter++;
    if (mFrameCounter == 7) mFrameCounter = 0;

    qDebug() << "ProduceTestFrame" << filename;

    QImage image1;
    image1.load(filename, "jpg");

    image1 = image1.scaled(mGrView->width(), mGrView->height());

    QGraphicsPixmapItem* v_pixmapItem = (QGraphicsPixmapItem*)mGrView->items().at(0); //FIXME
    v_pixmapItem->setPixmap(QPixmap::fromImage(image1));

    //mDrawWidget->setPixmap(QPixmap::fromImage(image1));

    //emit sigFrameReady(image1);
}

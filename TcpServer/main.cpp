#include "cls_tcpserver.h"

#include <QApplication>

#include "cls_boneservermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    cls_boneServerMainWindow mainWin;
    mainWin.show();

    return a.exec();
}

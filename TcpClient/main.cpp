#include <QApplication>

#include "cls_mdiwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    cls_mdiWindow* mainWin = new cls_mdiWindow();
    mainWin->show();

    // Rediect qDebug into the GUI text editor
    qInstallMessageHandler(cls_mdiWindow::myMessageOutput);

    return a.exec();
}

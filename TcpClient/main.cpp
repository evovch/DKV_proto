#include <QApplication>

#include "cls_formconnections.h"
#include "cls_serverfinder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cls_formConnections w1;
    cls_serverFinder w2;
    w1.show();
    w2.show();

    return a.exec();
}

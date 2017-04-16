#ifndef CLS_FORMCONNECTIONS_H
#define CLS_FORMCONNECTIONS_H

#include <QWidget>

namespace Ui {
    class cls_formConnections;
}

class cls_formConnections : public QWidget
{
    Q_OBJECT

public:
    explicit cls_formConnections(QWidget *parent = 0);
    ~cls_formConnections();

private:
    Ui::cls_formConnections *ui;

};

#endif // CLS_FORMCONNECTIONS_H

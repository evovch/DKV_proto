#ifndef CLS_MULTICLIENT_H
#define CLS_MULTICLIENT_H

#include <QTabWidget>

namespace Ui {
    class cls_multiClient;
}

class cls_multiClient : public QTabWidget
{
    Q_OBJECT

public:
    explicit cls_multiClient(QWidget *parent = 0);
    ~cls_multiClient();

public slots:
    void myAddTab();
    void myRemoveTab();

private:
    Ui::cls_multiClient *ui;

    unsigned int mTabUID;

};

#endif // CLS_MULTICLIENT_H

#include "cls_multiclient.h"
#include "ui_cls_multiclient.h"

#include "cls_tcpclient.h"

cls_multiClient::cls_multiClient(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::cls_multiClient)
{
    ui->setupUi(this);

    mTabUID = 0;
}

cls_multiClient::~cls_multiClient()
{
    delete ui;
}

void cls_multiClient::myAddTab()
{
    int newTabIndex;
    newTabIndex = this->addTab(new cls_tcpClient, QString::number(mTabUID));
    this->setCurrentIndex(newTabIndex);
    mTabUID++;
}

void cls_multiClient::myRemoveTab()
{
    cls_tcpClient* currentTab = (cls_tcpClient*)this->currentWidget();
    currentTab->on_pbDisconnect_clicked();
    this->removeTab(this->currentIndex());
}

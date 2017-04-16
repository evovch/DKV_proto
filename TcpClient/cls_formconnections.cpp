#include "cls_formconnections.h"
#include "ui_cls_formconnections.h"

cls_formConnections::cls_formConnections(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_formConnections)
{
    ui->setupUi(this);
}

cls_formConnections::~cls_formConnections()
{
    delete ui;
}

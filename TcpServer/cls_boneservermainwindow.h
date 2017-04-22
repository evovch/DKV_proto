#ifndef CLS_BONESERVERMAINWINDOW_H
#define CLS_BONESERVERMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class cls_boneServerMainWindow;
}

class cls_boneCamera;

class cls_boneServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cls_boneServerMainWindow(QWidget *parent = 0);
    ~cls_boneServerMainWindow();

    static cls_boneServerMainWindow* GetInstance();

private slots:
//    void on_pbStartLvLoop_clicked();
//    void on_pbStopLvLoop_clicked();

    void on_pushButton_clicked();

private:
    Ui::cls_boneServerMainWindow *ui;

    static cls_boneServerMainWindow* mInstance;

    cls_boneCamera* mCameraObj;

};

#endif // CLS_BONESERVERMAINWINDOW_H

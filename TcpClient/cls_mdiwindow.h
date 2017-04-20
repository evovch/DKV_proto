#ifndef CLS_MDIWINDOW_H
#define CLS_MDIWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
    class cls_mdiWindow;
}

class cls_mdiWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cls_mdiWindow(QWidget *parent = 0);
    ~cls_mdiWindow();

    static cls_mdiWindow* GetInstance();

    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:
    void sigNewLog(const QString& p_message);

public slots:
    void AppendToLog(const QString& p_message);

private slots:
    void on_textEdit_textChanged();

private:
    Ui::cls_mdiWindow *ui;

    static cls_mdiWindow* mInstance;

};

#endif // CLS_MDIWINDOW_H

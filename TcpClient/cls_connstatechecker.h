#ifndef CLS_CONNSTATECHECKER_H
#define CLS_CONNSTATECHECKER_H

#include <QObject>

/*#include <QWidget>

namespace Ui {
    class cls_connStateChecker;
}*/

class QTimer;

class cls_connStateChecker : public QObject /*QWidget*/
{
    Q_OBJECT

public:
    explicit cls_connStateChecker(QObject /*QWidget*/ *parent = 0);
    ~cls_connStateChecker();

public slots:
    void Start();
    void Stop();
    void SingleCheck();

signals:
    void finished();
    //void error(QString err);
    void performCheck();

private:
    /*Ui::cls_connStateChecker *ui;*/

    QTimer* mTimer;

    bool mStarted;

};

#endif // CLS_CONNSTATECHECKER_H

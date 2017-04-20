#include "cls_mdiwindow.h"
#include "ui_cls_mdiwindow.h"

#include <QtGlobal>

#include "cls_formconnections.h"
#include "cls_serverfinder.h"

#define MAXLOGTEXTSIZE 20000

// Singleton management
cls_mdiWindow* cls_mdiWindow::mInstance = nullptr;

cls_mdiWindow::cls_mdiWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cls_mdiWindow)
{
    ui->setupUi(this);

    // Singleton management
    if (mInstance == nullptr) {
        mInstance = this;
    } else {
        qDebug() << "Only one instance of cls_mdiWindow can exist!";
        return;
    }

    // Rediect qDebug into the GUI text editor
    connect(this, SIGNAL(sigNewLog(const QString&)), this, SLOT(AppendToLog(const QString&)));

    cls_formConnections* w1 = new cls_formConnections(this);
    cls_serverFinder* w2 = new cls_serverFinder(this);

    ui->mdiArea->addSubWindow(w1);
    ui->mdiArea->addSubWindow(w2);

    w1->show();
    w2->show();
}

cls_mdiWindow::~cls_mdiWindow()
{
    delete ui;

    // Singleton management
    mInstance = nullptr;

    //TODO check?
    //TODO delete w1, w2
}

// Singleton management
cls_mdiWindow* cls_mdiWindow::GetInstance()
{
    if (mInstance == nullptr) {
        return new cls_mdiWindow();
    } else {
        return mInstance;
    }
}

// Rediect qDebug into the GUI text editor
void cls_mdiWindow::myMessageOutput(QtMsgType /*type*/, const QMessageLogContext& /*context*/, const QString &msg)
{
    cls_mdiWindow* inst = cls_mdiWindow::GetInstance();
    emit inst->sigNewLog(msg);

/*    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
        break;
    default:
        break;
    }*/
}

// Rediect qDebug into the GUI text editor
void cls_mdiWindow::AppendToLog(const QString& p_message)
{
    ui->textEdit->append(p_message);
}

// Thx https://forum.qt.io/topic/23725/solved-limit-number-of-characters-in-qtextedit/3
void cls_mdiWindow::on_textEdit_textChanged()
{
    int curLogLen = ui->textEdit->toPlainText().length();
    if (curLogLen > MAXLOGTEXTSIZE) {
        int diff = curLogLen - MAXLOGTEXTSIZE;
        QString newStr = ui->textEdit->toPlainText();
        newStr.remove(0, diff);
        ui->textEdit->setText(newStr);
        QTextCursor cursor(ui->textEdit->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->textEdit->setTextCursor(cursor);
    }
}

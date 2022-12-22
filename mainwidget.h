#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QByteArray>
#include <QCryptographicHash>
#include <QSqlError>
#include <QtCore5Compat/QTextCodec>
#include <QtCore5Compat/QTextEncoder>
#include <QtCore5Compat/QTextDecoder>
#include <QCloseEvent>

#include "fileobject.h"
#include "readnotepad.h"
#include "appendnotepad.h"
#include "include/easylogging++.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    void openDB();
    ~MainWidget();


public slots:
    void getUsername(QString username, int degree);
    void updateFile();

protected:
    void closeEvent(QCloseEvent *event) override
    {
        LOG(INFO) << QString("User " + username + " session closed").toStdString();
        QWidget::closeEvent(event);

    }
private slots:
    void on_openButton_clicked();

    void on_readButton_clicked();

    void on_appendButton_clicked();

    void on_writeButton_clicked();

private:
    Ui::MainWidget *ui;
    QSqlDatabase dataDB;
    int degree;
    FileObject* obj;
    QString username;

};

#endif // MAINWIDGET_H

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <QCryptographicHash>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMessageBox>

#include "include/easylogging++.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    void openDB();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int getButtons();
    ~Login();


private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase passDB;
    QPoint mpos;
signals:
    void usernameSignal(QString username, int degree);
};

#endif // LOGIN_H

#ifndef ADMINPANELWIDGET_H
#define ADMINPANELWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDir>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QIdentityProxyModel>
#include <QtWidgets>
#include <QMap>
#include <QVariant>
#include <QIdentityProxyModel>

#include "include/easylogging++.h"

class PasswordEnter;

namespace Ui {
class AdminPanelWidget;
}

class AdminPanelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanelWidget(QWidget *parent = nullptr);
    ~AdminPanelWidget();
    QString new_pass;
    void set_new_pass(QString p);


private slots:
    void on_writeButton_clicked();

    void on_deleteButton_clicked();

    void on_pushButton_clicked();

    void on_changePassButton_clicked();

    void on_deleteUserButton_clicked();

    void on_addUserButton_clicked();

    void on_writeButton_2_clicked();

private:
    Ui::AdminPanelWidget *ui;
    QSqlDatabase dataDB;
    QSqlDatabase passDB;
    QSqlTableModel *dataModel;
    QSqlTableModel *dataModel2;
};

#endif // ADMINPANELWIDGET_H

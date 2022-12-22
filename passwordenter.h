#ifndef PASSWORDENTER_H
#define PASSWORDENTER_H

#include <QDialog>
#include <QMessageBox>
#include <QCryptographicHash>

#include "include/easylogging++.h"

class AdminPanelWidget;

namespace Ui {
class PasswordEnter;
}

class PasswordEnter : public QDialog
{
    Q_OBJECT

public:
    PasswordEnter(QString title, AdminPanelWidget *parent = nullptr);
    ~PasswordEnter();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PasswordEnter *ui;
    AdminPanelWidget *par;
};

#endif // PASSWORDENTER_H

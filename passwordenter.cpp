#include "adminpanelwidget.h"
#include "passwordenter.h"
#include "ui_passwordenter.h"

PasswordEnter::PasswordEnter(QString title, AdminPanelWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordEnter)
{
    ui->setupUi(this);
    this->par = parent;
    this->setWindowTitle(title);
}

PasswordEnter::~PasswordEnter()
{
    delete ui;
}

void PasswordEnter::on_buttonBox_accepted()
{
    if (ui->lineEdit->text() == ui->lineEdit_2->text())
    {
        QString hash_pass = QString(QCryptographicHash::hash((this->ui->lineEdit->text().toUtf8()),QCryptographicHash::Md5).toHex());
        this->par->set_new_pass(hash_pass);
        this->done(1);
    }
    else
    {
        QMessageBox::warning(this, "Предупреждение!", "Пароли не совпадают!");
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
}


void PasswordEnter::on_buttonBox_rejected()
{
    this->done(0);
}


#include "login.h"
#include "ui_login.h"


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
    passDB.close();
}


void Login::on_okButton_clicked()
{
    QString hash_pass = QString(QCryptographicHash::hash((this->ui->pass_line->text().toUtf8()),QCryptographicHash::Md5).toHex());
    QString user = this->ui->user_line->text();
    QSqlQuery qry;
    qry = passDB.exec("SELECT pass, degree FROM PASSWORDS WHERE login = \'" + user + "\'");
    if (!qry.first())
    {
        LOG(INFO) << QString("Someone tried to log as " + user).toStdString();
        QMessageBox::warning(this, "Предупреждение!", "Такого пользователя нет!");
        ui->pass_line->clear();
        ui->user_line->clear();
    }
    else {
        if (qry.value(0) == hash_pass) {

            ui->pass_line->clear();
            ui->user_line->clear();
            int wantedDegree =this->getButtons();
            if (qry.value(1).toInt() >= wantedDegree)
            {
                emit usernameSignal(user, wantedDegree);
                this->done(wantedDegree);

            }

            else
            {
                LOG(INFO) << QString("User " + user + " tried to get degree " + QString::number(wantedDegree)).toStdString();
                QMessageBox::warning(this, "Предупреждение!", "Доступ на данный уровень не разрешен!");
            }
        }
        else
        {
            LOG(INFO) << QString("User " + user + " entered wrong password").toStdString();
            QMessageBox::warning(this, "Предупреждение!", "Неправильный пароль!");
            ui->pass_line->clear();
        }
    }
    qry.finish();
}


void Login::on_cancelButton_clicked()
{
    this->done(0);
}

void Login::openDB()
{
    passDB = QSqlDatabase::addDatabase("QSQLITE", "PASS");
    QString pathToDB = QDir::currentPath() + QString("/db/pass.sqlite");
    passDB.setDatabaseName(pathToDB);

    QFileInfo checkFile(pathToDB);

    if (checkFile.exists()) {
        if (passDB.open()) {
            QMessageBox::information(this, "OK!", "Успешное подключение к базе данных!");
        }
        else {
            QMessageBox::critical(this, "Ошибка!", "База данных не была открыта!");
            ui->okButton->setEnabled(false);
        }
    }
    else {
        QMessageBox::critical(this, "Ошибка!", "Файл базы данных не существует!");
        ui->okButton->setEnabled(false);
    }
}
void Login::mousePressEvent(QMouseEvent *event)
{
    mpos = event->pos();
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}
int Login::getButtons()
{
    if (ui->degree1_button->isChecked())
        return 1;
    if (ui->degree2_button->isChecked())
        return 2;
    if (ui->degree3_button->isChecked())
        return 3;
    if (ui->degree4_button->isChecked())
        return 4;
    else
        return 0;
}

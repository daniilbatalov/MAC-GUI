#include "mainwidget.h"
#include "ui_mainwidget.h"

QString degreeToString(int deg)
{
    switch (deg)
    {
    case 1:
        return "уровень «общий»";
    case 2:
        return "уровень «для служебного пользования»";
    case 3:
        return "уровень «конфиденциально»";
    case 4:
        return "режим администратора";
    default:
        return "";
    }
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

}

MainWidget::~MainWidget()
{
    delete ui;
    dataDB.close();
}

void MainWidget::updateFile()
{
    QByteArray data;
    QFile file(this->obj->getPath());
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    QString hash_pass = QString((QCryptographicHash::hash(data, QCryptographicHash::Md5)).toHex());
    QDate cd = QDate::currentDate();
    QTime ct = QTime::currentTime();
    QString timedate = ct.toString("HH:mm") + " " + cd.toString("dd.MM.yyyy");
    dataDB = QSqlDatabase::addDatabase("QSQLITE", "DATA");
    QString pathToDB = QDir::currentPath() + QString("/db/data.sqlite");
    dataDB.setDatabaseName(pathToDB);
    dataDB.open();
    QSqlQuery qry;
    qry = dataDB.exec("UPDATE files SET md5 = \'" + hash_pass + "\' WHERE path = \'" + this->obj->getPath() + "\'");
    qry = dataDB.exec("UPDATE files SET editedBy = \'" + this->username + "\' WHERE path = \'" + this->obj->getPath() + "\'");
    qry = dataDB.exec("UPDATE files SET editedAt = \'" + timedate + "\' WHERE path = \'" + this->obj->getPath() + "\'");
    dataDB.close();
    LOG(INFO) << QString("User " + this->username + " edited file " + this->obj->getPath()).toStdString();
}

void MainWidget::getUsername(QString username, int degree)
{
    this->username = username;
    this->degree = degree;
    LOG(INFO) << QString("User " + this->username + ", degree " + QString::number(this->degree) + " session started").toStdString();
    this->setWindowTitle("MAC: пользователь " + username + ", " + degreeToString(this->degree));
}

void MainWidget::on_openButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName;
    if (dialog.exec())
        fileName = dialog.selectedFiles().at(0);
    ui->path_lineedit->setText(fileName);
    QByteArray data;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    QString hash_pass = QString((QCryptographicHash::hash(data, QCryptographicHash::Md5)).toHex());
    QSqlQuery qry;
    qry = dataDB.exec("SELECT md5, degree, editedBy, editedAt FROM files WHERE path = \'" + fileName + "\'");

    if (!qry.next())
    {
        LOG(INFO) << QString("User " + this->username + " tried to open file that isn't in database").toStdString();
        QMessageBox::warning(this, "Предупреждение!", "Этого файла нет в базе данных. Обратитесь к администратору!");
    }
    else
    {
        if (qry.value(0) == hash_pass)
        {
            LOG(INFO) << QString("User " + this->username + " opened file " + fileName + " info").toStdString();
            this->obj = new FileObject(fileName, qry.value(0).toString(), qry.value(1).toInt());
            ui->infoLabel2->setText("Уровень доступа: " + degreeToString(this->obj->getDegree()) + "\nРедактирован пользователем: " + qry.value(2).toString() + "\nВремя последнего редактирования: " + qry.value(3).toString());
            ui->readButton->setEnabled(true);
            ui->writeButton->setEnabled(true);
            ui->appendButton->setEnabled(true);
        }
        else
        {
            LOG(INFO) << QString("File " + fileName + " is corrupted.").toStdString();
            QMessageBox::warning(this, "Предупреждение!", "Файл поврежден или скомпрометирован. Обратитесь к администратору!");
        }
    }
}

void MainWidget::openDB()
{
    dataDB = QSqlDatabase::addDatabase("QSQLITE", "DATA");
    QString pathToDB = QDir::currentPath() + QString("/db/data.sqlite");
    dataDB.setDatabaseName(pathToDB);

    QFileInfo checkFile(pathToDB);

    if (checkFile.exists()) {
        if (dataDB.open()) {
            QMessageBox::information(this, "OK!", "Успешное подключение к базе данных!");
        }
        else {
            QMessageBox::critical(this, "Ошибка!", "База данных не была открыта!");
            ui->readButton->setEnabled(false);
            ui->writeButton->setEnabled(false);
            ui->openButton->setEnabled(false);
        }
    }
    else {
        QMessageBox::critical(this, "Ошибка!", "Файл базы данных не существует!");
        ui->readButton->setEnabled(false);
        ui->writeButton->setEnabled(false);
        ui->openButton->setEnabled(false);
    }
}

void MainWidget::on_readButton_clicked()
{
    if (this->obj->getDegree() <= this->degree)
    {
        LOG(INFO) << QString("User " + this->username + " read file " + this->obj->getPath()).toStdString();
        ReadNotepad *r = new ReadNotepad(obj->getPath());
        r->show();
    }
    else
    {
        LOG(INFO) << QString("User " + this->username + " tried to read file " + this->obj->getPath()).toStdString();
        QMessageBox::warning(this, "Предупреждение!", "Вы не можете открыть данный файл! Минимальный уровень: " + degreeToString(this->obj->getDegree()));
    }
}


void MainWidget::on_appendButton_clicked()
{
    if (this->obj->getDegree() >= this->degree)
    {
        LOG(INFO) << QString("User " + this->username + " appended file " + this->obj->getPath()).toStdString();
        AppendNotepad *a = new AppendNotepad(true, obj->getPath());
        QObject::connect(a, &AppendNotepad::file_edited, this, &MainWidget::updateFile);
        a->show();
    }
    else
    {
        LOG(INFO) << QString("User " + this->username + " tried to append file " + this->obj->getPath()).toStdString();
        QMessageBox::warning(this, "Предупреждение!", "Вы не можете дописывать данный файл! Максимальный уровень: " + degreeToString(this->obj->getDegree()));
    }
}


void MainWidget::on_writeButton_clicked()
{
    if (this->obj->getDegree() == this->degree)
    {
        LOG(INFO) << QString("User " + this->username + " rewrited file " + this->obj->getPath()).toStdString();
        AppendNotepad *a = new AppendNotepad(false, obj->getPath());
        QObject::connect(a, &AppendNotepad::file_edited, this, &MainWidget::updateFile);
        a->show();
    }
    else
    {
        LOG(INFO) << QString("User " + this->username + " tried to rewrite file " + this->obj->getPath()).toStdString();
        QMessageBox::warning(this, "Предупреждение!", "Вы не можете записывать в данный файл! Требуемый уровень: " + degreeToString(this->obj->getDegree()));
    }
}


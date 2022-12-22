#include "appendnotepad.h"
#include "ui_appendnotepad.h"

AppendNotepad::AppendNotepad(bool way, QString path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppendNotepad)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Дописать");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    this->file = new QFile(path);
    this->setWindowTitle("MAC: дозапись " + path);
    this->mode = way;
    if (!this->mode)
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Записать");
        this->setWindowTitle("MAC: запись " + path);
        this->file->open(QIODevice::ReadOnly);
        QByteArray old_text = this->file->readAll();
        ui->plainTextEdit->document()->setPlainText(old_text);
        this->file->flush();
        this->file->close();
    }
}

AppendNotepad::~AppendNotepad()
{
    delete ui;
}

void AppendNotepad::on_buttonBox_accepted()
{
    if (this->mode)
    {
        if (this->file->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QString data_qstr = "\n" + ui->plainTextEdit->toPlainText();
            this->file->write(data_qstr.toStdString().c_str());
            this->file->flush();
            this->file->close();
            emit file_edited();
        }
    }
    else
    {
        if (this->file->open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            QString data_qstr = ui->plainTextEdit->toPlainText();
            this->file->write(data_qstr.toStdString().c_str());
            this->file->flush();
            this->file->close();
            emit file_edited();
        }
    }
}


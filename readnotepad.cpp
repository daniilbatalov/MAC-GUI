#include "readnotepad.h"
#include "ui_readnotepad.h"

ReadNotepad::ReadNotepad(QString path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadNotepad)
{
    ui->setupUi(this);
    QFile* f = new QFile(path);
    f->open(QIODevice::ReadOnly);
    QByteArray old_text = f->readAll();
    ui->plainTextEdit->document()->setPlainText(old_text);
    this->setWindowTitle("MAC: чтение " + path);
}

ReadNotepad::~ReadNotepad()
{
    delete ui;
}

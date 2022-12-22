#ifndef APPENDNOTEPAD_H
#define APPENDNOTEPAD_H

#include <QDialog>
#include <QPushButton>
#include <QFile>

namespace Ui {
class AppendNotepad;
}

class AppendNotepad : public QDialog
{
    Q_OBJECT

public:
    explicit AppendNotepad(bool, QString, QWidget *parent = nullptr);
    ~AppendNotepad();

private slots:
    void on_buttonBox_accepted();

signals:
    void file_edited();

private:
    Ui::AppendNotepad *ui;
    QFile* file;
    bool mode;
};

#endif // APPENDNOTEPAD_H

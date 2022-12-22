#ifndef READNOTEPAD_H
#define READNOTEPAD_H

#include <QDialog>
#include <QFile>

namespace Ui {
class ReadNotepad;
}

class ReadNotepad : public QDialog
{
    Q_OBJECT

public:
    explicit ReadNotepad(QString path, QWidget *parent = nullptr);
    ~ReadNotepad();

private:
    Ui::ReadNotepad *ui;
};

#endif // READNOTEPAD_H

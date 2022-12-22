#ifndef FILEOBJECT_H
#define FILEOBJECT_H

#include <QString>
#include <QByteArray>

class FileObject
{
public:
    FileObject(QString, QString, int);
    QString getPath();
    QString getMD5();
    int getDegree();
private:
    QString path;
    QString md5;
    int degree;

};

#endif // FILEOBJECT_H

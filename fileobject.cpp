#include "fileobject.h"

FileObject::FileObject(QString path, QString md5, int degree)
{
    this->path = path;
    this->degree = degree;
    this->md5 = md5;
}

QString FileObject::getPath()
{
    return this->path;
}
QString FileObject::getMD5()
{
    return this->md5;
}
int FileObject::getDegree()
{
    return this->degree;
}

#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include <iostream>

class Utils : public QObject
{
    Q_OBJECT

public:
    static QString getFileContent(QString);
    static bool isFileFromExtension(QString, QString);

private:

};

#endif // UTILS_H

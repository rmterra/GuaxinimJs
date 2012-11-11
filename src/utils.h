#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include <iostream>

class Utils : public QObject
{
    Q_OBJECT

public:
    static QString getContent(QString);
    static bool isExtension(QString, QString);

private:

};

#endif // UTILS_H

#include "utils.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <iostream>

//PUBLIC STATIC
QString Utils::getContent(QString filePath) {
    if(!QFile::exists(filePath)) {
        std::cout << "File not found..." << std::endl;
        return "";
    }

    QFile *file = new QFile(filePath);

    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Couldn't open the file..." << std::endl;
        return "";
    }

    QTextStream in(file);
    QString fileContent = in.readAll();
    if(fileContent.isEmpty()) return "";
    return fileContent;
}

bool Utils::isExtension(QString filePath, QString extension) {
    QFileInfo fileInfo(filePath);
    if(fileInfo.suffix() == extension) {
        return true;
    }
    return false;
}

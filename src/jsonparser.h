#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QMap>
#include <QScriptValue>

class JSONParser : public QObject
{
    Q_OBJECT

public:
    static QString toString(QScriptValue);
    static QMap<QString, QString> toMap(QScriptValue);

};

#endif // JSONPARSER_H

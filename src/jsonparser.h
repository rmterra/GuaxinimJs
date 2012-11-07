#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QScriptValue>

class JSONParser : public QObject
{
    Q_OBJECT

public:
    static QString toString(QScriptValue);

};

#endif // JSONPARSER_H

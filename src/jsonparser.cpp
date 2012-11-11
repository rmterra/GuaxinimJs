#include <QScriptValueIterator>

#include "jsonparser.h"

QString JSONParser::toString(QScriptValue json) {

    if(!json.isObject()) return "";

    QScriptValueIterator iterator(json);
    QString jsonParsed;

    while(iterator.hasNext()) {
        iterator.next();

        jsonParsed.append(iterator.name());
        jsonParsed.append(":");

        QScriptValue value = iterator.value();

        if(value.isObject()) {
            toString(value);
        } else if (value.isString()){
            QString stringValue("'%1'");
            jsonParsed.append(stringValue.arg(value.toString()));
        } else {
            jsonParsed.append(value.toString());
        }

        if(iterator.hasNext()) {
            jsonParsed.append(", ");
        }
    }

    QString finalParse("{ %1 }");
    return finalParse.arg(jsonParsed);
}

QMap<QString, QString> JSONParser::toMap(QScriptValue json) {
    QMap<QString, QString> map;
    QScriptValueIterator iterator(json);

    if(!json.isObject()) return map;

    while(iterator.hasNext()) {
        iterator.next();
        map.insert(iterator.name(), iterator.value().toString());
    }

    return map;
}

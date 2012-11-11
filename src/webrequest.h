#ifndef WEBREQUEST_H
#define WEBREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QScriptValue>
#include <QUrl>
#include <QMap>
#include <QScriptEngine>

class WebRequest : public QObject
{
    Q_OBJECT
public:
    WebRequest(QScriptEngine*);
    WebRequest(QScriptEngine*, QScriptValue);

private:
    QScriptEngine *m_engine;
    QUrl m_url;
    QMap<QString, QString> m_argsMap;
    QByteArray m_verb;
    QScriptValue m_onSuccess;
    QScriptValue m_onError;
    QNetworkAccessManager* m_manager;

public slots:
    void sendRequest();

private slots:
    void executeFinished(QNetworkReply*);

signals:
    void requestFinished(QScriptValue);

};

#endif // WEBREQUEST_H

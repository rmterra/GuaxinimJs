#ifndef WEBREQUEST_H
#define WEBREQUEST_H

#include <QUrl>
#include <QMap>
#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QNetworkAccessManager>

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

private:
    void configHeaders(QNetworkRequest*);

public slots:
    void sendRequest();

private slots:
    void executeFinished(QNetworkReply*);

signals:
    void requestFinished(QScriptValue);

};

#endif // WEBREQUEST_H

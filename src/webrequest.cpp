#include <QDebug>
#include <QNetworkReply>

#include "webrequest.h"
#include "src/jsonparser.h"
#include "src/const.h"

//PUBLIC METHODS

WebRequest::WebRequest(QScriptEngine *engine)
    : QObject()
    , m_engine(engine)
{
}

WebRequest::WebRequest(QScriptEngine *engine, QScriptValue parameters)
    : QObject()
    , m_engine(engine)
{
    QString url = parameters.property("url").toString();
    this->m_manager = new QNetworkAccessManager(this);
    this->m_url = QUrl(url);
    this->m_verb = parameters.property("verb").toString().toAscii().constData();
    this->m_onSuccess = parameters.property("success");
    this->m_onError = parameters.property("error");
    this->m_argsMap = JSONParser::toMap(parameters.property("data"));
}

//PRIVATE METHODS

void WebRequest::configHeaders(QNetworkRequest* request) {
    request->setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request->setRawHeader("Accept-Charset", "ISO-8859-1, utf-8; q=0.7, *; q=0.3");
    request->setRawHeader("Accept-Encoding", "gzip, deflate, sdch");
    request->setRawHeader("Accept-Language", "en-US, en; q=0.8");

    QByteArray userAgent = "GuaxinimJs ";
    userAgent.append(GUAXINIMJS_VERSION_STRING);

    request->setRawHeader("User-Agent", userAgent);
}

//PUBLIC SLOTS

void WebRequest::sendRequest() {
    foreach (QString key, this->m_argsMap.keys()) {
        this->m_url.addQueryItem(key, this->m_argsMap[key]);
    }

    QNetworkRequest request(this->m_url);
    configHeaders(&request);

    connect(this->m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(executeFinished(QNetworkReply*)));
    this->m_manager->sendCustomRequest(request, this->m_verb);
}

//PRIVATE SLOTS

void WebRequest::executeFinished(QNetworkReply* reply) {
    if(reply->error() != QNetworkReply::NoError) {
        this->m_onError.call(QScriptValue(), QScriptValueList() << reply->errorString());
        return;
    }

    QString result = reply->readAll();
    QScriptValue json = this->m_engine->evaluate("("+ result +")");
    this->m_onSuccess.call(QScriptValue(), QScriptValueList() << json);
}

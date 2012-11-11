#include <iostream>

#include <QDebug>
#include <QFileInfo>
#include <QWebFrame>

#include "src/webpage.h"
#include "src/utils.h"
#include <src/jsonparser.h>

WebPage::WebPage(QWebPage *page, QObject *parent)
    : QObject(parent)
    , m_page(page)
{
    setObjectName(this->url());
}

//PUBLIC METHODS

QString WebPage::url() const {
    return this->m_page->mainFrame()->url().toString();
}

QWebPage* WebPage::page() const {
    return this->m_page;
}

//PULBIC SLOTS

void WebPage::evaluate(QString script) {
    QString finalScript("(%1)()");
    finalScript = finalScript.arg(script);
    this->m_page->mainFrame()->evaluateJavaScript(finalScript);
}

void WebPage::evaluate(QScriptValue args, QString script) {
    QString finalScript("(%1).call(%2);");
    QString json = JSONParser::toString(args);

    finalScript = finalScript.arg(script, json);
    this->m_page->mainFrame()->evaluateJavaScript(finalScript);
}

QScriptValue WebPage::inject(QString file) {
    if(!Utils::isExtension(file, QString("js"))) {
        return QScriptValue(false);
    }

    QString script = Utils::getContent(file);

    if(script.isEmpty()) { return false; }

    this->m_page->mainFrame()->evaluateJavaScript(script);
    return QScriptValue(true);
}

void WebPage::setHtml(QString file) {
    QFileInfo fileInfo(file);

    if(!fileInfo.isFile()) {
        this->m_page->mainFrame()->setHtml(file);
        return;
    }

    if(!Utils::isExtension(file, QString("html")) ||
       !Utils::isExtension(file, QString("htm"))) {
        std::cout << "Invalid file " << file.toAscii().constData() << std::endl;
        return;
    }

    QString content = Utils::getContent(file);
    this->m_page->mainFrame()->setHtml(content);
}

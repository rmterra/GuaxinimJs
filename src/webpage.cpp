/*
    This file is part of the GuaxinimJs project.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
          names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

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

#include <QDebug>
#include <QWebFrame>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <src/jsws.h>
#include <src/webpage.h>
#include <src/webrequest.h>

//PUBLIC CONSTRUCTORS

Jsws::Jsws(QScriptEngine* engine, QObject *parent)
    : QObject(parent)
    , m_async(true)
    , m_keyPosition(0)
    , m_engine(engine)
    , m_script("")
{
    init();
}

Jsws::Jsws(QScriptEngine* engine, QString script, QObject *parent)
    : QObject(parent)
    , m_async(true)
    , m_keyPosition(0)
    , m_engine(engine)
    , m_script(script)
{
    init();
}

//PUBLIC METHODS

void Jsws::async(bool async) {
    this->m_async = async;
}

bool Jsws::isAsync() const {
    return this->m_async;
}

QStringList Jsws::args() const {
    return this->m_args;
}

QScriptValue Jsws::page() const {
    QScriptValue object = this->m_engine->newQObject(this->m_page);
    return object;
}

//PRIVATE METHODS

void Jsws::init() {
    qDebug("Initializing the Framework");
    setObjectName("App_Core");
    m_args = QApplication::arguments();
    connect(this, SIGNAL(completeScrape()), this, SLOT(executeCompleteScrape()));

    QWebPage *page = new QWebPage();
    configPage(page);
    page->mainFrame()->addToJavaScriptWindowObject("WS", this);
    this->m_page = new WebPage(page, this);
}

void Jsws::configPage(QWebPage *page) {

    QWebSettings *settings = page->settings();

    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::AutoLoadImages, true);

    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanCloseWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
}

void Jsws::proccessAsync() {
    foreach (QString url, this->m_pages.keys()) {
        QWebPage *page = this->m_pages.value(url);
        connect(page, SIGNAL(loadProgress(int)), this, SLOT(pageLoadProgress(int)));
        connect(page, SIGNAL(loadFinished(bool)), this, SLOT(pageLoadFinished(bool)));
        page->mainFrame()->setUrl(QUrl(url));
    }
}

void Jsws::proccessSync() {
    if(this->m_pages.count() == 0 || this->m_keyPosition >= this->m_pages.count()) {
        emit completeScrape();
        return;
    }

    this->m_nextWebPage = this->m_pages.value(this->m_pages.keys().at(m_keyPosition));

    connect(this->m_nextWebPage, SIGNAL(loadProgress(int)), this, SLOT(pageLoadProgress(int)));
    connect(this->m_nextWebPage, SIGNAL(loadFinished(bool)), this, SLOT(pageLoadFinished(bool)));
    QUrl url(this->m_pages.key(this->m_nextWebPage));
    this->m_nextWebPage->mainFrame()->setUrl(url);
}

//PUBLIC SLOTS

void Jsws::scrape(QString url, QScriptValue args) {
    scrape(QStringList() << url, args);
}

void Jsws::scrape(QStringList urls, QScriptValue args) {
    try {
        qDebug() << "Entered on Scrape";
        if(urls.isEmpty()) {
            qDebug() << "URL vector is empty... Aborting operation";
            return;
        }

        if(!args.isObject()) {
            qDebug() << "Arguments in incorrect format... Aborting operation";
            return;
        }

        this->m_defaultParams = RequestParams::fromScriptObject(args);
        this->m_keyPosition = 0;

        foreach (QString url, urls) {
            QWebPage *page = new QWebPage(this);
            configPage(page);
            page->mainFrame()->addToJavaScriptWindowObject("WS", this);
            this->m_pages.insert(url,page);
        }

        this->m_defaultParams->isAsync() ?
            proccessAsync() :
            proccessSync();
    } catch (...) {
        qFatal("Something wrong ocurred and the application will stop...");
        QApplication::instance()->exit(0);
    }
}

void Jsws::request(QScriptValue args) {
    WebRequest *request = new WebRequest(this->m_engine, args);
    request->sendRequest();
}

void Jsws::toPdf(QString) {

}

void Jsws::toImage(QString) {

}

void Jsws::render(int width, int height) {
    QWebView *view = new QWebView();
    view->setPage(this->m_page->page());
    view->setFixedWidth(width);
    view->setFixedHeight(height);
    view->show();
}

void Jsws::print(QString message) {
    std::cout << message.toAscii().constData() << std::endl;
}

void Jsws::clrscr() {
#ifdef WINDOWS
    system("CLS");
#else
    system("clear");
#endif
}

void Jsws::exit() {
    qDeleteAll(m_pages);
    m_pages.clear();
    QApplication::instance()->exit();
}

//PRIVATE SLOTS

void Jsws::pageLoadProgress(int progress) {
    QScriptValue callback = this->m_defaultParams->progress();
    if(callback.isNull() || callback.isUndefined()) {
        return;
    }

    QWebPage *page = qobject_cast<QWebPage *>(QObject::sender());
    QScriptValue ctx = this->m_engine->globalObject().property("WS");
    QString host = page->mainFrame()->baseUrl().host();
    callback.call(ctx, QScriptValueList() << progress << host);
}

void Jsws::pageLoadFinished(bool success) {
    QScriptValue callback = this->m_defaultParams->finish();
    if(callback.isNull() || callback.isUndefined()) {
        return;
    }

    QWebPage *page = qobject_cast<QWebPage *>(QObject::sender());

    disconnect(page,SIGNAL(loadProgress(int)),0,0);
    disconnect(page,SIGNAL(loadFinished(bool)),0,0);

    WebPage *webPage = new WebPage(page,this);
    QScriptValue pageArg = this->m_engine->newQObject(webPage);

    QScriptValue ctx = this->m_engine->globalObject().property("WS");
    callback.call(ctx, QScriptValueList() << success << pageArg);

    if(!this->m_defaultParams->isAsync()) {
        this->m_keyPosition++;
        this->proccessSync();

    } else {
        if(++this->m_keyPosition == this->m_pages.count()) {
            emit completeScrape();
            return;
        }
    }
}

void Jsws::executeCompleteScrape() {
    QScriptValue callback = this->m_defaultParams->complete();

    qDeleteAll(m_pages);
    m_pages.clear();

    if(callback.isNull() || callback.isUndefined()) {
        return;
    }

    QString result("(%1)()");
    this->m_engine->evaluate(result.arg(callback.toString()));
}

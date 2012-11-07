#include <QDebug>
#include <QApplication>
#include <QWebPage>
#include <QWebFrame>
#include <QScriptEngine>
#include <QScriptValue>

#include <src/jsws.h>
#include <src/webpage.h>

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
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanCloseWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
}

void Jsws::proccessAsync() {
    foreach (QString url, this->m_pages.keys()) {
        QWebPage *page = this->m_pages.value(url);
        connect(page, SIGNAL(loadProgress(int)), this, SLOT(pageLoadProgress(int)));
        connect(page, SIGNAL(loadFinished(bool)), this, SLOT(pageLoadFinished(bool)));
        qDebug() << QString("Loading %1").arg(url);
        page->mainFrame()->setUrl(QUrl(url));
    }
}

void Jsws::proccessSync() {
    if(this->m_pages.count() == 0) {
        //TODO ERROR
    }
    if(this->m_keyPosition >= this->m_pages.count()) {
        emit completeScrape();
        return;
    }

    this->m_nextWebPage = this->m_pages.value(this->m_pages.keys().at(m_keyPosition));

    connect(this->m_nextWebPage, SIGNAL(loadProgress(int)), this, SLOT(pageLoadProgress(int)));
    connect(this->m_nextWebPage, SIGNAL(loadFinished(bool)), this, SLOT(pageLoadFinished(bool)));
    QUrl url(this->m_pages.key(this->m_nextWebPage));
    qDebug() << QString("Loading %1").arg(url.toString());

    this->m_nextWebPage->mainFrame()->setUrl(url);
}

//PUBLIC SLOTS

void Jsws::scrape(QStringList urls, QScriptValue args) {
    try {
        qDebug("Entered on Scrape");
        if(urls.isEmpty()) {
            qDebug("URL vector is empty... Aborting operation");
            return;
        }

        if(!args.isObject()) {
            qDebug("Arguments in incorrect format... Aborting operation");
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

void Jsws::request(QScriptValue) {

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
    QApplication::setQuitOnLastWindowClosed(false);
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
    }
    else {
        if(++this->m_keyPosition == this->m_pages.count()) {
            emit completeScrape();
            return;
        }
    }
}

void Jsws::executeCompleteScrape() {
    QScriptValue callback = this->m_defaultParams->complete();
    if(callback.isNull() || callback.isUndefined()) {
        return;
    }

    QString result("(%1)()");
    this->m_engine->evaluate(result.arg(callback.toString()));
}

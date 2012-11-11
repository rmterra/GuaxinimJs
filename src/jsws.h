#ifndef JSWS_H
#define JSWS_H

#include <iostream>

#include <QObject>
#include <QWebView>
#include <QWebPage>
#include <QScriptValue>
#include <QScriptEngine>

#include <src/requestparams.h>
#include <src/webpage.h>

class Jsws : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool async WRITE async READ isAsync)
    Q_PROPERTY(QStringList args READ args)
    Q_PROPERTY(QScriptValue page READ page)

public:
    explicit Jsws(QScriptEngine*, QObject *parent = 0);
    explicit Jsws(QScriptEngine*, QString, QObject *parent = 0);

public:
    void async(bool) ;
    bool isAsync() const;

    QStringList args() const;

    QScriptValue page() const;

private:
    bool m_async;
    int m_keyPosition;

    QStringList m_args;
    QScriptEngine *m_engine;
    QString m_script;
    RequestParams *m_defaultParams;
    QMap<QString, QWebPage*> m_pages;
    QWebPage *m_nextWebPage;
    WebPage *m_page;

private:
    void init();
    void configPage(QWebPage*);
    void proccessAsync();
    void proccessSync();

public slots:
    void scrape(QString, QScriptValue);
    void scrape(QStringList, QScriptValue);
    void request(QScriptValue);

    void toPdf(QString);
    void toImage(QString);

    void render(int, int);

    void print(QString);
    void clrscr();


private slots:
    void pageLoadProgress(int);
    void pageLoadFinished(bool);
    void executeCompleteScrape();
    void exit();

signals:
    void completeScrape();

};

#endif // JSWS_H

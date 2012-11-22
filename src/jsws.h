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

/**
  * The heart of GuaxinimJs, this class represent the main namespace of the project.
  */
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
    /**
      * Set if the default scrape proccess is async or not
      *
      * @todo Implement the use of this property
      * @brief async
      * @param value - new value
      */
    void async(bool value) ;
    /**
      * Set if the default scrape proccess is async or not
      *
      * @todo Implement the use of this property
      * @brief isAsync
      * @return Boolean true if the scrape proccess is async
      */
    bool isAsync() const;
    /**
      * Recover the list of arguments used to run the script.
      * The list is expected in this format:
      * [ pathToApp, optionsUsedToRun, scriptFileName, otherArgs... ]
      *
      * @brief args
      * @return String List with args passed on line command when the script startup
      */
    QStringList args() const;
    /**
      * The page used for output of engine
      *
      * @see WebPage class for more details
      * @brief page
      * @return QscriptValue based on QWebPage class
      */
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
    /**
      * Start the web scrape/crawling proccess based on a list of url
      *
      * @brief scrape
      * @param urls
      * @param options
      */
    void scrape(QStringList ulrs, QScriptValue options);
    void request(QScriptValue);

    void toPdf(QString);
    void toImage(QString);

    void render(int, int);

    void print(QString);

    void exit();

private slots:
    void pageLoadProgress(int);
    void pageLoadFinished(bool);
    void executeCompleteScrape();

signals:
    void completeScrape();

};

#endif // JSWS_H

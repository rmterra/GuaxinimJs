#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QObject>
#include <QWebPage>
#include <QScriptValue>

class WebPage : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QWebPage page READ page SCRIPTABLE false)

public:
    explicit WebPage(QWebPage*, QObject *parent = 0);

public:
    QString url() const;

    QWebPage* page() const;

private:
    QWebPage *m_page;

public slots:
    void evaluate(QString);
    void evaluate(QScriptValue, QString);
    QScriptValue inject(QString);
    void setHtml(QString);

};

#endif // WEBPAGE_H

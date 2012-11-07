#ifndef REQUESTPARAMS_H
#define REQUESTPARAMS_H

#include <QObject>
#include <QMap>
#include <QScriptValue>

#define ASYNC "async"
#define PROGRESS "progress"
#define FINISH "finish"
#define COMPLETE "complete"

class RequestParams : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool async READ isAsync WRITE setAsync)
    Q_PROPERTY(QScriptValue progress READ progress WRITE setProgress)
    Q_PROPERTY(QScriptValue finish READ finish WRITE setFinish)
    Q_PROPERTY(QScriptValue complete READ complete WRITE setComplete)

public:
    explicit RequestParams(QObject *parent = 0);

private:
    bool m_async;
    QScriptValue m_progress;
    QScriptValue m_finish;
    QScriptValue m_complete;

public:
    bool isAsync() const;
    void setAsync(bool);

    QScriptValue progress() const;
    void setProgress(QScriptValue);

    QScriptValue finish() const;
    void setFinish(QScriptValue);

    QScriptValue complete() const;
    void setComplete(QScriptValue);

public:
    static RequestParams* fromScriptObject(QScriptValue);
};

#endif // REQUESTPARAMS_H

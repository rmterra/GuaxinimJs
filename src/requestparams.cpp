#include <QVariant>

#include "requestparams.h"

RequestParams::RequestParams(QObject *parent)
    : QObject(parent)
    , m_async(true)
    , m_progress("")
    , m_finish("")
{}

//PUBLIC

bool RequestParams::isAsync() const {
    return m_async;
}

void RequestParams::setAsync(bool value) {
    m_async = value;
}

QScriptValue RequestParams::progress() const {
    return m_progress;
}

void RequestParams::setProgress(QScriptValue callback) {
    m_progress = callback;
}

QScriptValue RequestParams::finish() const {
    return m_finish;
}

void RequestParams::setFinish(QScriptValue callback) {
    m_finish = callback;
}

QScriptValue RequestParams::complete() const {
    return m_complete;
}

void RequestParams::setComplete(QScriptValue callback) {
    m_complete = callback;
}

RequestParams* RequestParams::fromScriptObject(QScriptValue literalObj) {
    RequestParams *result = new RequestParams();
    result->setAsync(literalObj.property(ASYNC).toBool());
    result->setProgress(literalObj.property(PROGRESS));
    result->setFinish(literalObj.property(FINISH));
    result->setComplete(literalObj.property(COMPLETE));
    return result;
}


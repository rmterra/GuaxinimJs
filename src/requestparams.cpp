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


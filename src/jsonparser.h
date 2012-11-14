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

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QMap>
#include <QObject>
#include <QScriptValue>

/**
  * A bunch of useful static functions to convert from QScriptValue Json based objects to another types
  * This class shouldn't be instantiated
  */
class JSONParser : public QObject
{
    Q_OBJECT

private:
    JSONParser();

public:
    /**
      * Convert the QScriptValue Json based on a literal string
      *
      * For instance the return will be:
      * { foo: 'Hello World', bar: { value: 10 }, vect: [0,1,2] }
      *
      * @brief toString
      * @param json - Json based object to convert
      * @return QString that represent the literal json object
      */
    static QString toString(QScriptValue json);
    /**
      * Convert the QScriptValue Json based on a literal string
      *
      * @brief toString
      * @param json - Json based object to convert
      * @return QString that represent the literal json object
      */
    static QMap<QString, QString> toMap(QScriptValue json);
};

#endif // JSONPARSER_H

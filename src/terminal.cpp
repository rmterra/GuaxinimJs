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

#include "terminal.h"

//PUBLIC

Terminal::Terminal(QObject *parent)
  : QObject(parent)
  , m_appReturn(0)
{
    this->m_engine = new QScriptEngine();
}

bool Terminal::execute(QStringList args) {

    if(args.count() == NO_ARGUMENTS_COUNT) {
        execInteractiveInstance();
        return true;
    }

    for(int i = 1; i < args.count(); i++) {
        std::string str = args[i].toLocal8Bit().constData();

        if(str == "--file" || str == "-f") {
            if(args.count() == i+1) {
                std::cout << "Please specify the file..." << std::endl;
                m_appReturn = -1;
                return false;
            }
            bool result = execFile(args[i+1]);
            return result;
        }

        if(str == "--version" || str == "-v") { execVersion(); return true; }
        if(str == "--help" || str == "-h") { execHelp(); return true; }
    }
    std::cout << "JsWebScrape: invalid option.\n";
    std::cout << "Try './JsWebScrape --help' for more information." << std::endl;
    return false;
}

int Terminal::appReturn() {
    return m_appReturn;
}

//PRIVATE

void Terminal::execInteractiveInstance() {
    //TODO Terminal::execInteractiveInstance()
}

void Terminal::execHelp() {
    std::cout << "Usage: ./jsws [OPTION]" << std::endl;
    std::cout << "Possible arguments:" << std::endl;
    std::cout << "-f, --file [FILE_PATH] execute program with specified script" << std::endl;
    std::cout << "-v, --version show jsws version and some other details" << std::endl;
    m_appReturn = -1;
}

bool Terminal::execFile(QString filePath) {
    if(!Utils::isExtension(filePath, QString("js"))) {
        std::cout << "File Invalid..."
                  << std::endl
                  << "Acceptable extension *.js";
        return false;
    }

    QString script = Utils::getContent(filePath);

    if(script.isEmpty()) { return false; }

    QScriptSyntaxCheckResult result = this->m_engine->checkSyntax(script);

    if(result.state() == QScriptSyntaxCheckResult::Error) {
        QString error("Error on line %1 column %2\nMessage: %3");
        error = error.arg(QString::number(result.errorLineNumber()),
                          QString::number(result.errorColumnNumber()),
                          result.errorMessage());

        std::cout << error.toAscii().constData() << std::endl;
        return false;
    }

    Jsws *jsws = new Jsws(this->m_engine, script,parent());
    QScriptValue value = this->m_engine->newQObject(jsws);
    this->m_engine->globalObject().setProperty("WS", value);
    this->m_engine->evaluate(script);
    return true;
}

void Terminal::execVersion() {
    std::cout << GUAXINIMJS_VERSION_STRING << std::endl;
}

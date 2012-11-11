#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>

#include <QFile>
#include <QObject>
#include <QFileInfo>
#include <QTextStream>
#include <QStringList>
#include <QScriptEngine>

#include <src/const.h>
#include <src/utils.h>
#include <src/jsws.h>

#define NO_ARGUMENTS_COUNT 1

class Terminal : public QObject
{
    Q_OBJECT
public:
    explicit Terminal(QObject *parent = 0);

    bool execute(QStringList);
    int appReturn();

private:
    void execHelp();
    bool execFile(QString);
    void execVersion();
    void execInteractiveInstance();

    QScriptEngine *m_engine;
    int m_appReturn;


signals:

public slots:

};

#endif // TERMINAL_H

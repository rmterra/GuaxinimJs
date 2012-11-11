#include <iostream>

#include <QtGui/QApplication>
#include <QStringList>

#include <src/terminal.h>
#include <src/const.h>

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);
    QStringList args = app->arguments();

    app->setWindowIcon(QIcon("racccoon.png"));
    app->setApplicationName("GuaxinimJs");
    app->setApplicationVersion(GUAXINIMJS_VERSION_STRING);

    Terminal *terminal = new Terminal(app);

    if(terminal->execute(args)) {
        app->exec();
    }

    int retVal = terminal->appReturn();
    delete terminal;
#ifdef WINDOWS
    return retVal;
#else
    return (retVal);
#endif
}

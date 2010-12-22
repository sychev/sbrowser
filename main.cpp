#include <QtGui/QApplication>
#include <QTextCodec>
#include <QDir>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mainwindow.h"
#include "singleapplication.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    FILE *debugOutput;
    if((debugOutput = fopen("./debug.log", "a")) == NULL) {
          return;
    }

    switch (type) {
        case QtDebugMsg:
            fprintf(debugOutput, "[%3d.%03d]Debug: %s\n", (int)clock() / 1000, (int)clock() % 1000, msg);
            break;
        case QtWarningMsg:
            fprintf(debugOutput, "[%3d.%03d]Warning: %s\n", (int)clock() / 1000, (int)clock() % 1000, msg);
            break;
        case QtCriticalMsg:
            fprintf(debugOutput, "[%3d.%03d]Critical: %s\n", (int)clock() / 1000, (int)clock() % 1000, msg);
            break;
        case QtFatalMsg:
            fprintf(debugOutput, "[%3d.%03d]Fatal: %s\n", (int)clock() / 1000, (int)clock() % 1000, msg);
            abort();
    }
    fclose(debugOutput);
    /*
    */
}

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv, "f35fe4692ace47cb2390a381ccbd30ae"); // md5 hash of "sbrowserSytchev"

    if (app.isRunning())
    {
            app.sendMessage("Hello. I'm the other instance of sbrowser. I'll die. Bye.");
            return 0;
    }

    QDir::setCurrent(QCoreApplication::applicationFilePath().remove(QRegExp("/[^/]+$")));

    // set local codec
    QTextCodec *cyrillicCodec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(cyrillicCodec);
    QTextCodec::setCodecForLocale(cyrillicCodec);
    QTextCodec::setCodecForCStrings(cyrillicCodec);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

    qInstallMsgHandler(myMessageOutput);

    bool silentStart = false;
    //qDebug() << "Параметры старта всего" << argc;
    QString testplanDirectoryKey("-d");
    QString testplanFileKey("-f");
    for(int i = 0; i < argc; ++i) {
        if (QString(argv[i]) == testplanDirectoryKey) {
            silentStart = true;
        }
        if (QString(argv[i]) == testplanFileKey) {
            silentStart = true;
        }
        //qDebug() << "Параметр [" << i << "] =" << argv[i];
    }


    MainWindow *mainWindow = new MainWindow();

    // Канал связи между приложениями
    QObject::connect(&app, SIGNAL(messageAvailable(QString)), mainWindow, SLOT(receiveApplicationMessage(QString)));

    if (!silentStart)
        mainWindow->show();

    int returnCode;
    returnCode = app.exec();

    delete mainWindow;
    return returnCode;
}

#ifndef SBROWSERWIDGET_H
#define SBROWSERWIDGET_H

#include <QtGui>
#include <QtCore>
#include <QDateTime>
#include <QtWebKit>

#ifndef RELEASE
#include "debugwidget.h"
#endif


class SbrowserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SbrowserWidget(QWidget *parent = 0);
    ~SbrowserWidget();

protected:
    void resizeEvent(QResizeEvent* /*event*/);
    void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    void showHelp();
    void debugSlot1();
    void debugSlot2();

private slots:
    void onUrlChanged(QUrl url);
    void loadSadadDev();
    void showPostParameters();
    void showSourceCode();
    void runTest();
    void goBack();
    void loadUrlFromLine();

private:
    void readProgramSettings();
    void writeProgramSettings();

    QLabel *regexpLabel;
    QLineEdit *urlLineEdit;
    QTextEdit *postVarsTextEdit;
    QWebView *webView;
    QLabel *browserBorderLabel;
    QTextEdit *htmlSourceTextEdit;

#ifndef RELEASE
    void setupDebugWidget();
    DebugWidget *debugWidget;
#endif
    void loadUrl(QString urlString);
    QStringList inputNameList;
    QString formActionTarget;
    QString oldPostVarsText;
    bool showOldPostVars;
    int postVarsTextCursorPosition;

};

#endif // SBROWSERWIDGET_H

#ifndef LEVELBUZZWIDGET_H
#define LEVELBUZZWIDGET_H

#include <QtGui>
#include <QtCore>
#include <QDateTime>
#include <QtWebKit>

#include "debugwidget.h"


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
    void goBack();

private:
    void readProgramSettings();
    void writeProgramSettings();

    QLabel *regexpLabel;
    QLineEdit *urlLineEdit;
    QTextEdit *postVarsTextEdit;
    QWebView *webView;
    QLabel *browserBorderLabel;
    QTextEdit *htmlSourceTextEdit;

    void setupDebugWidget();
    DebugWidget *debugWidget;
    void runTest();
    void loadUrl(QString urlString);
    QStringList inputNameList;
    QString formActionTarget;
    QString oldPostVarsText;
    bool showOldPostVars;
    int postVarsTextCursorPosition;

};

#endif // LEVELBUZZWIDGET_H

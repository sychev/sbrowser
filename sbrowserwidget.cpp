#include "sbrowserwidget.h"

SbrowserWidget::SbrowserWidget(QWidget *parent) :
    QWidget(parent)
{
    showOldPostVars = false;

    QFont monotypeFont("Courier New", 10);
    regexpLabel = new QLabel(this);
    regexpLabel->setText(tr("Url"));
    regexpLabel->hide();

    urlLineEdit = new QLineEdit(this);
    urlLineEdit->setFont(monotypeFont);
    urlLineEdit->setText("http://google.com/");

    postVarsTextEdit = new QTextEdit(this);
    postVarsTextEdit->setFont(monotypeFont);
    postVarsTextEdit->setWordWrapMode(QTextOption::NoWrap);

    htmlSourceTextEdit = new QTextEdit(this);

    webView = new QWebView(this);
    webView->load(QUrl("http://google.com/"));
    webView->page()->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    connect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(showPostParameters()));
    connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(showSourceCode()));
    webView->show();

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F1), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showHelp()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_F2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(loadSadadDev()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_F3), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showPostParameters()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(goBack()));

    browserBorderLabel = new QLabel(this);
    browserBorderLabel->setObjectName("browserBorderLabel");
    browserBorderLabel->lower();

    readProgramSettings();
#ifndef RELEASE
    setupDebugWidget();
#endif
}

SbrowserWidget::~SbrowserWidget()
{
    writeProgramSettings();
}

void SbrowserWidget::resizeEvent(QResizeEvent* /*event*/)
{
    urlLineEdit->setGeometry(10, 10, width() - 20, 24);
    postVarsTextEdit->setGeometry(10, 40, 400, 200);
    htmlSourceTextEdit->setGeometry(10, 250, 400, height() - 260);
    webView->setGeometry(422, 42, width() - 434, height() - 54);
    browserBorderLabel->setGeometry(420, 40, width() - 430, height() - 50);
}

void SbrowserWidget::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter)) {
        if (event->modifiers() & Qt::ControlModifier) {
// Ctrl + Enter
            runTest();
        } else {
            loadUrlFromLine();
// Enter
        }
    }
    if((event->key() == Qt::Key_L) && (event->modifiers() & Qt::ControlModifier)) {
// Ctrl + L
        urlLineEdit->setFocus();
        urlLineEdit->selectAll();
    }
    if (event->modifiers() & Qt::AltModifier) {
        if(event->key() == Qt::Key_Left) {
// Alt + Left
            webView->back();
        }
        if(event->key() == Qt::Key_Right) {
// Alt + Right
            webView->forward();
        }
    }
}

void SbrowserWidget::runTest()
{
    postVarsTextCursorPosition = postVarsTextEdit->textCursor().position();
    QString postVarsText = postVarsTextEdit->toPlainText();
    oldPostVarsText = postVarsText;
    QStringList postVarsList = postVarsText.split("\n", QString::SkipEmptyParts);
    postVarsText.clear();
    QString postVarName, postVarValue;
    QRegExp postVarRegExp("\\$_POST\\[\"([^\"]+)\"\\] *= *\"(.*)\";");
    QRegExp postTargetRegExp("action *= *[\"\']([^\"^\']*)[\"\']");
    foreach(QString postVar, postVarsList) {
        if (postVarRegExp.indexIn(postVar, 0) != -1) {
            postVarName = postVarRegExp.cap(1);
            postVarValue = postVarRegExp.cap(2);
            postVarsText += postVarName + "=" + postVarValue + "&";
        }
        if (postTargetRegExp.indexIn(postVar, 0) != -1) {
            formActionTarget = postTargetRegExp.cap(1);
        }
    }
    postVarsText.remove(QRegExp("&$"));

    QNetworkRequest request;
    request.setUrl(QUrl(formActionTarget));
    //qDebug() << postVarsText;

    webView->load(request, QNetworkAccessManager::PostOperation, postVarsText.toAscii());
}

void SbrowserWidget::onUrlChanged(QUrl url)
{
    urlLineEdit->setText(url.toString());
}

void SbrowserWidget::loadUrl(QString urlString)
{
    if (!urlString.contains(QRegExp("[^:]+://"))) {
        urlString = urlString.prepend("http://");
    }
    webView->load(QUrl(urlString));
}

void SbrowserWidget::loadSadadDev()
{
    loadUrl("http://google.com/");
}

void SbrowserWidget::showPostParameters()
{
    if (showOldPostVars) {
        showOldPostVars = false;
        postVarsTextEdit->setPlainText(oldPostVarsText);
        QTextCursor textCursor = postVarsTextEdit->textCursor();
        textCursor.setPosition(postVarsTextCursorPosition);
        postVarsTextEdit->setTextCursor(textCursor);
    } else {
        QString htmlSource;
        formActionTarget.clear();
        QWebPage *webPage = webView->page();
        QWebFrame *webFrame = webPage->mainFrame();
        htmlSource = webFrame->toHtml();
        QRegExp formRegExp;
        //formRegExp.setPattern("(<form.*method.*=.*\"post\"[\\s\\S]*)<form>");
        //while (formRegExp.indexIn(htmlSource, 0) != -1) {
        //    htmlSource = formRegExp.cap(1);
        //}
        formRegExp.setPattern("(<form.*method.*=.*\"post\".*</form>)");
        if (formRegExp.indexIn(htmlSource, 0) != -1) {
            htmlSource = formRegExp.cap(1);
        }
        //qDebug() << htmlSource;

        inputNameList.clear();
        formRegExp.setPattern("input[^>]*name=[\'\"]([^\"^\']+)[\'\"]");
        int pos = 0;
        QString inputName;
        while ((pos = formRegExp.indexIn(htmlSource, pos)) != -1) {
            inputName = formRegExp.cap(1);
            if (!inputNameList.contains(inputName)) {
                inputNameList.append(inputName);
                //qDebug() << pos << inputName;
            }
            pos += formRegExp.matchedLength();
        }
        formRegExp.setPattern("select[^>]*name=[\'\"]([^\"^\']+)[\'\"]");
        pos = 0;
        while ((pos = formRegExp.indexIn(htmlSource, pos)) != -1) {
            inputName = formRegExp.cap(1);
            if (!inputNameList.contains(inputName)) {
                inputNameList.append(inputName);
                //qDebug() << pos << inputName;
            }
            pos += formRegExp.matchedLength();
        }

        formRegExp.setPattern("action=\"([^\"]+)\"");
        if (formRegExp.indexIn(htmlSource, 0) != -1) {
            formActionTarget = formRegExp.cap(1);
        }
        QString baseUrlString = webFrame->url().toString();
        if (formActionTarget.startsWith('/')) {
            QRegExp baseRegExp("(http://[^/]+)");
            if (baseRegExp.indexIn(baseUrlString, 0) != -1) {
                baseUrlString = baseRegExp.cap(1);
            }
        }
        if (!formActionTarget.startsWith("http://"))
            formActionTarget.prepend(baseUrlString);

        QString postVarsText;
        postVarsText += "action = \"" + formActionTarget + "\"\n";
        foreach (QString name, inputNameList) {
            postVarsText += "$_POST[\"" + name + "\"] = \"\";\n";
        }
        postVarsTextEdit->setPlainText(postVarsText);
        //qDebug() << formActionTarget;
    }
}

void SbrowserWidget::showSourceCode()
{
    QString htmlSource;
    QWebPage *webPage = webView->page();
    QWebFrame *webFrame = webPage->mainFrame();
    htmlSource = webFrame->toHtml();
    htmlSourceTextEdit->setPlainText(htmlSource);
}

void SbrowserWidget::goBack()
{
    showOldPostVars = true;
    webView->back();
}

void SbrowserWidget::showHelp()
{
    QMessageBox::about(this, tr("SBrowser help"),
    tr("<h1>SBrowser 0.1.1</h1>"
    "<h2>Help</h2>"
    "<p><b>F1</b> - show this windows</p>"
    "<p><b>F2</b> - load google.com</p>"
    "<p><b>F3</b> - show POST parameters of current page</p>"
    "<p><b>Ctrl + Enter</b> - send POST request</p>"
    "<p><b>F5</b> - back, and show last POST parameters</p>"
    "<h2>POST examples</h2>"
    "<p>$_POST[\"username\"] = \"<b>alex</b>\";<br />username = alex</p>"
    "<p>$_POST[\"pwd\"] = \"<b>5\" screen isn't too big</b>\";<br />pwd = 5\" screen isn't too big</p>"
    "<h2>Browsing</h2>"
    "<p><b>Enter</b> - load page</p>"
    "<p><b>Ctrl + L</b> - edit address line</p>"
    "<p><b>Alt + Left</b> - go back</p>"
    "<p><b>Alt + Right</b> - go forward</p>"
    "<h2>Author</h2>"
    "<p><b>Alex Sychev</b></p>"
    "<p><b>mail:</b> <a href='mailto:194145@gmail.com'>194145@gmail.com</a></p>"
    "<p><b>site:</b> <a href='http://sychev.com/'>sychev.com</a></p>"));
}

void SbrowserWidget::readProgramSettings()
{
}

void SbrowserWidget::writeProgramSettings()
{
}

#ifndef RELEASE
void SbrowserWidget::setupDebugWidget()
{
    debugWidget = new DebugWidget;
    debugWidget->setWindowFlags(/*Qt::Popup | */Qt::WindowStaysOnTopHint);
    //debugWidget->show();
    QShortcut *debugShortcut = new QShortcut(QKeySequence(Qt::CTRL /*+ Qt::ALT*/ + Qt::Key_D), this);
    connect(debugShortcut, SIGNAL(activated()), debugWidget, SLOT(show()));
    //connect(someWidget, SIGNAL(liveMesageChanged(QString)), debugWidget, SLOT(setLiveMessage(QString)));
    //connect(debugWidget, SIGNAL(printDebugInfoButtonClicked()), this, SLOT(printDebugInformation()));
    connect(debugWidget, SIGNAL(debugButtonClicked1()), this, SLOT(debugSlot1()));
    connect(debugWidget, SIGNAL(debugButtonClicked2()), this, SLOT(debugSlot2()));
    //connect(debugWidget, SIGNAL(debugButtonClicked3()), this, SLOT(debugSlot3()));
    //connect(debugWidget, SIGNAL(debugButtonClicked4()), this, SLOT(debugSlot4()));
}
#endif

void SbrowserWidget::debugSlot1()
{
}

void SbrowserWidget::debugSlot2()
{
}

void SbrowserWidget::loadUrlFromLine()
{
    loadUrl(urlLineEdit->text());
}

#include "debugwidget.h"

DebugWidget::DebugWidget(QWidget *parent) :
    QFrame(parent)
{
    // ��������������� ��������� �������
    readSettings();
    setWindowTitle(tr("Regexptest debug mode"));

    // ������� ����������
    liveMessage = new QLabel(tr("����� ��������"), this);

    clearLogButton = new QPushButton(tr("�������� ���"), this);
    printDebugInfoButton = new QPushButton(tr("������"), this);

    /*
    */
    debugButton1 = new QPushButton(tr("Add"), this);
    debugButton2 = new QPushButton(tr("Clear"), this);
    debugButton3 = new QPushButton(tr("3"), this);
    debugButton4 = new QPushButton(tr("4"), this);

    // ������ ���������
    connect(clearLogButton, SIGNAL(clicked()), this, SLOT(clearDebugLog()));
    connect(printDebugInfoButton, SIGNAL(clicked()), this, SIGNAL(printDebugInfoButtonClicked()));

    /*
    */
    connect(debugButton1, SIGNAL(clicked()), this, SIGNAL(debugButtonClicked1()));
    connect(debugButton2, SIGNAL(clicked()), this, SIGNAL(debugButtonClicked2()));
    connect(debugButton3, SIGNAL(clicked()), this, SIGNAL(debugButtonClicked3()));
    connect(debugButton4, SIGNAL(clicked()), this, SIGNAL(debugButtonClicked4()));

    // ������������� ������������ ��������
    buttonLayout1 = new QHBoxLayout;
    buttonLayout1->addWidget(clearLogButton);
    buttonLayout1->addWidget(printDebugInfoButton);
    buttonLayout1->addWidget(debugButton1);
    buttonLayout1->addWidget(debugButton2);
    buttonLayout1->addWidget(debugButton3);
    buttonLayout1->addWidget(debugButton4);

    /*
    buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(debugButton1);
    buttonLayout2->addWidget(debugButton2);
    buttonLayout2->addWidget(debugButton3);
    buttonLayout2->addWidget(debugButton4);
    */

    debugLog = new QPlainTextEdit;
    debugLog->setReadOnly(true);
    debugLog->setWordWrapMode(QTextOption::NoWrap);

    layout = new QVBoxLayout;
    layout->addWidget(liveMessage);
    layout->addLayout(buttonLayout1);
    //layout->addLayout(buttonLayout2);
    layout->addWidget(debugLog);


    // ��������� � ��������� �����
    QFile qssFile(":/qss/qss/debugWidget.qss");
    qssFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(qssFile.readAll());
    setStyleSheet(styleSheet);

    // ��������� ������������
    setLayout(layout);

    // �������� ������ ���������� ����� ���������
    debugUpdateTimer = new QTimer(this);
    connect(debugUpdateTimer, SIGNAL(timeout()), this, SLOT(readDebug()));
    debugUpdateTimer->start(1000);

    QFile debugLogFile("./debug.log");
    debugLogFile.remove();

    QShortcut *debugShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(debugShortcut, SIGNAL(activated()), this, SLOT(close()));
}

void DebugWidget::readDebug()
{
    QFile debugLogFile;
    // ������������� ���� � ����� � ������� � ���������
    debugLogFile.setFileName("./debug.log");
    // ���� ���� ��������
    if (debugLogFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString newDebugLog = debugLogFile.readAll();
        if (newDebugLog.size() != debugLog->toPlainText().size()) {
            debugLog->setPlainText("");
            debugLog->appendPlainText(newDebugLog);
        }
        debugLogFile.close();
    } else {
        debugLog->setPlainText(tr("������ ����� ���������..."));
    }
    debugLog->moveCursor(QTextCursor::End);
}

void DebugWidget::clearDebugLog()
{
    QFile debugLogFile("./debug.log");
    debugLogFile.remove();
    readDebug();
}

// �������������� ���������� ��������� ����
void DebugWidget::readSettings()
{
    QSettings settings("sytchev.ru", "regexptest");
    restoreGeometry(settings.value("DebugWidget/geometry").toByteArray());
}

void DebugWidget::closeEvent(QCloseEvent *event)
{
    // ������ ��������� ����
    QSettings settings("sytchev.ru", "regexptest");
    settings.setValue("DebugWidget/geometry", saveGeometry());
    hide();
    event->ignore();
}

void DebugWidget::setLiveMessage(QString newLiveMessage)
{
    liveMessage->setText(newLiveMessage);
}

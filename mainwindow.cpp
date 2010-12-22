#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    readWindowSettings();
    setWindowTitle(tr("Sbrowser"));
    //createTrayIcon();

    // Таймер запоминания состояния
    writeWindowSettingsTimer = new QTimer(this);
    connect(writeWindowSettingsTimer, SIGNAL(timeout()), this, SLOT(writeWindowSettings()));
    writeWindowSettingsTimer->start(30 * 1000);

    sbrowserWidget = new SbrowserWidget(this);
    setCentralWidget(sbrowserWidget);

    // Прочитаем и установим стили
    QFile qssFile(":/qss/qss/sbrowserWidget.qss");
    qssFile.open(QFile::ReadOnly);
    styleSheet = QLatin1String(qssFile.readAll());
    setStyleSheet(styleSheet);
}

MainWindow::~MainWindow()
{
    writeWindowSettings();
    delete sbrowserWidget;
}

void MainWindow::receiveApplicationMessage(QString message)
{
    if (message == "Hello. I'm the other instance of sbrowser. I'll die. Bye.")
        showWindow();
}

void MainWindow::showWindow()
{
    if (isMinimized()) {
        showNormal();
        readWindowSettings();
    } else if (isHidden()) {
        show();
    }
    activateWindow();
}

void MainWindow::readWindowSettings()
{
    QSettings settings("sytchev.ru", "sbrowser");
    if (settings.value("MainWindow/geometry").toByteArray().size() != 0) {
        restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    } else {
        QDesktopWidget *desktop;
        desktop = QApplication::desktop();
        QRect availableRect = desktop->availableGeometry();
        if ((availableRect.width() > 800) && (availableRect.height() > 600)) {
            setGeometry(availableRect.x() + 60, availableRect.y() + 60, availableRect.width() - 120, availableRect.height() - 120);
        } else {
            setGeometry(availableRect);
        }
    }
    restoreState(settings.value("MainWindow/windowState").toByteArray());
}

void MainWindow::writeWindowSettings()
{
    // Запись положения окна
    QSettings settings("sytchev.ru", "sbrowser");
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
}

void MainWindow::showBuildNumberInTitle(QString buildNumber)
{
    if (!buildNumber.isEmpty()) {
        setWindowTitle(tr("Levelbuzz test shell. Build #") + buildNumber);
    } else {
        setWindowTitle(tr("Levelbuzz test shell."));
    }
}

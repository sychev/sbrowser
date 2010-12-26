#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sbrowserwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void receiveApplicationMessage(QString message);

private slots:
    void writeWindowSettings();

private:
    QTimer *writeWindowSettingsTimer;
    void readWindowSettings();
    void showWindow();

    SbrowserWidget *sbrowserWidget;
    QString styleSheet;
};

#endif // MAINWINDOW_H

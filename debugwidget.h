#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QtGui>

class DebugWidget : public QFrame
{
Q_OBJECT
public:
    explicit DebugWidget(QWidget *parent = 0);

public slots:
    void setLiveMessage(QString);

signals:
    void printDebugInfoButtonClicked();

    void debugButtonClicked1();
    void debugButtonClicked2();
    void debugButtonClicked3();
    void debugButtonClicked4();

private slots:
    void readDebug();
    void clearDebugLog();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void readSettings(); // читаем последние сохраненные настройки

    QLabel *liveMessage;
    QPushButton *clearLogButton;
    QPushButton *printDebugInfoButton;

    /*
    */
    QPushButton *debugButton1;
    QPushButton *debugButton2;
    QPushButton *debugButton3;
    QPushButton *debugButton4;

    QPlainTextEdit *debugLog;

    QHBoxLayout *buttonLayout1;
    //QHBoxLayout *buttonLayout2;
    QVBoxLayout *layout;

    QTimer *debugUpdateTimer;
};

#endif // DEBUGWIDGET_H

QT       += core gui network webkit

TARGET = sbrowser
TEMPLATE = app

release {
    DEFINES += RELEASE
}

SOURCES += main.cpp \
        mainwindow.cpp \
	singleapplication.cpp \
        sbrowserwidget.cpp

HEADERS  += mainwindow.h \
        singleapplication.h \
        sbrowserwidget.h

RC_FILE += icon.rc # данные о значке

RESOURCES += \
    sbrowser.qrc

OTHER_FILES += \
    qss/sbrowserWidget.qss
release {
OTHER_FILES += qss/debugWidget.qss
SOURCES += debugwidget.cpp
HEADERS += debugwidget.h
}

QT       += core gui network webkit

TARGET = sbrowser
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp\
	singleapplication.cpp \
    debugwidget.cpp \
    sbrowserwidget.cpp

HEADERS  += mainwindow.h\
        singleapplication.h \
    debugwidget.h \
    sbrowserwidget.h

RC_FILE += icon.rc # данные о значке

RESOURCES += \
    sbrowser.qrc

OTHER_FILES += \
    qss/sbrowserWidget.qss \
    qss/debugWidget.qss

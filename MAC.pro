QT       += core gui
QT       += sql
QT       += core5compat
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpanelwidget.cpp \
    appendnotepad.cpp \
    fileobject.cpp \
    include/easylogging++.cc \
    login.cpp \
    main.cpp \
    mainwidget.cpp \
    passwordenter.cpp \
    readnotepad.cpp

HEADERS += \
    adminpanelwidget.h \
    appendnotepad.h \
    fileobject.h \
    include/easylogging++.h \
    login.h \
    mainwidget.h \
    passwordenter.h \
    readnotepad.h

FORMS += \
    adminpanelwidget.ui \
    appendnotepad.ui \
    login.ui \
    mainwidget.ui \
    passwordenter.ui \
    readnotepad.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=


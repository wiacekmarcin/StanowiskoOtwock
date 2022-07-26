#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T17:08:19
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PrzeplywomierzTest

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    impulsydlg.cpp \
        main.cpp \
        mainwindow.cpp \
        ../Aplikacja/crc8.cpp \
        ../Aplikacja/mechanika.cpp \
    roletakroki.cpp \
    ../Aplikacja/serialmessage.cpp \
    ../Aplikacja/serialdevice.cpp \
    ../Aplikacja/ustawienia.cpp \
	../Aplikacja/rs232.cpp
HEADERS += \
    impulsydlg.h \
        mainwindow.h \
        ../Aplikacja/crc8.h \
        ../Aplikacja/mechanika.h \
    roletakroki.h \
    ../Aplikacja/serialmessage.h \
    ../Aplikacja/serialdevice.h \
    ../Aplikacja/ustawienia.h \
	../Aplikacja/rs232.h

FORMS += \
        impulsydlg.ui \
        mainwindow.ui \
        roletakroki.ui

win32 {
     !contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L$$PWD/../../setupapilib/32/setupapi/ -lsetupAPI
        LIBS += -L$$PWD/../../setupapilib/32/setupapi/ -ladvAPI32

    } else {
        LIBS += -L$$PWD/../../setupapilib/64/setupapi/ -lsetupAPI
        LIBS += -L$$PWD/../../setupapilib/64/setupapi/ -ladvAPI32
    }
}
linux {
        DEFINES += SERIALLINUX
}

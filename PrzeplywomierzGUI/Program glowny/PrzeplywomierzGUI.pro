#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T17:08:19
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PrzeplywomierzGUI
TEMPLATE = app

TRANSLATIONS    +=  gui_pl.ts

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
        main.cpp \
        crc8.cpp \
        mechanika.cpp \
    pozycjeroleta.cpp \
    serialmessage.cpp \
    ustawienia.cpp \
    wyborkwadratow.cpp \
    wybormetody.cpp \
    wybranyplik.cpp \
    miernikprzeplywu.cpp \
    pozycje.cpp \
    recznedodpozycji.cpp \
    mierzonepozycje.cpp \
    sterownik.cpp \
    etapyroletdlg.cpp
HEADERS += \
        crc8.h \
        mechanika.h \
    pozycjeroleta.h \
    serialmessage.h \
    ustawienia.h \
    wyborkwadratow.h \
    wybormetody.h \
    wybranyplik.h \
    miernikprzeplywu.h \
    pozycje.h \
    recznedodpozycji.h \
    mierzonepozycje.h \
    sterownik.h \
    etapyroletdlg.h

FORMS += \
    pozycjeroleta.ui \
    wybordanych.ui \
    wyborkwadratow.ui \
    wybormetody.ui \
    wybranyplik.ui \
    miernikprzeplywu.ui \
    recznedodpozycji.ui \
    mierzonepozycje.ui \
    etapyroletdlg.ui

DISTFILES += \
    polozenia.txt

#-------------------------------------------------
#
# Project created by QtCreator 2024-08-23T10:53:52
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += texttospeech
QT       += multimedia
QT       += multimediawidgets
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project-1
TEMPLATE = app

INCLUDEPATH += C:/OpenSSL-Win32/include

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
        widget.cpp \
    hisui.cpp \
    aksk.cpp

HEADERS += \
        widget.h \
    hisui.h \
    aksk.h

FORMS += \
        widget.ui \
    hisui.ui \
    aksk.ui

RESOURCES += \
    img.qrc

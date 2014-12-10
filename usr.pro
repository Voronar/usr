#-------------------------------------------------
#
# Project created by Kirill Khalitov A. 2014-12-10T16:19:04
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    device_settings.cpp

HEADERS  += mainwindow.h \
    device_settings.h

FORMS    += mainwindow.ui \
    device_settings.ui

CONFIG += C++11

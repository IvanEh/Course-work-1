#-------------------------------------------------
#
# Project created by QtCreator 2015-05-24T22:43:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Course-work
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    main.cpp \
    systemlinearequation.cpp \
    systemsolver.cpp \
    gaussianelimination.cpp \
    jacobisolver.cpp \
    gaussseidelsolver.cpp \
    sorsolver.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    systemsolver.h \
    gaussianelimination.h \
    jacobisolver.h \
    gaussseidelsolver.h \
    sorsolver.h \
    linear-system.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

CONFIG += c++11

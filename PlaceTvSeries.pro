#-------------------------------------------------
#
# Project created by QtCreator 2014-05-09T19:31:37
#
# Copyright 2016 Luca Angioloni
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Organizer
TEMPLATE = app


SOURCES += main.cpp\
    filter.cpp \
    filters.cpp \
    info.cpp \
    preferences.cpp \
    organizer.cpp \
    filterdatabase.cpp \
    filterwindow.cpp \
    progressbar.cpp \
    copythread.cpp \
    scanningwindow.cpp \
    filelist.cpp \
    poolchecker.cpp \
    scantask.cpp

HEADERS  += \
    filter.h \
    filters.h \
    info.h \
    preferences.h \
    organizer.h \
    filterdatabase.h \
    filterwindow.h \
    progressbar.h \
    copythread.h \
    scanningwindow.h \
    filelist.h \
    poolchecker.h \
    scantask.h

FORMS    += \
    filters.ui \
    info.ui \
    preferences.ui \
    organizer.ui \
    filterwindow.ui \
    progressbar.ui \
    scanningwindow.ui

RESOURCES += \
    Files.qrc

ICON = icon1.icns


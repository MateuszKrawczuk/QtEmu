##
## QtEmu - A front-end for qemu emulator
##
## Copyright (C) 2006-2008 Urs Wolfer <uwolfer @ fwo.ch>
## Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU Library General Public License
## along with this library; see the file COPYING.LIB.  If not, write to
## the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.
##
##-------------------------------------------------
##
## Project created by the awesome QtCreator
##
##-------------------------------------------------

message("Generating Makefile for QtEmu... $$escape_expand(\\n)")

QT += core gui widgets
message("Building with Qt v$$QT_VERSION")

lessThan(QT_MAJOR_VERSION, 5) {
    warning(" >>> You're trying to build with Qt 4")
    warning(" >>> This version of QtEmu requires Qt 5")
    warning(" >>> You might need to use qmake-qt5 instead $$escape_expand(\\n)")

    error("Aborting!")
}

TARGET = qtemu

TEMPLATE = app

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/helpwidget.cpp \
           src/aboutwidget.cpp \
    src/configwindow.cpp

HEADERS  += src/mainwindow.h \
            src/helpwidget.h \
            src/aboutwidget.h \
    src/configwindow.h

OTHER_FILES += \
    CHANGELOG \
    LICENSE \
    qtemu.desktop \
    README \
    TODO \
    translations
    
RESOURCES += qtemu.qrc



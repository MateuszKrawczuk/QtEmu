##
## QtEmu - A front-end for qemu emulator
##
## Copyright (C) 2006-2008 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
## Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
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

QT += core gui widgets network
message("Building with Qt v$$QT_VERSION")

lessThan(QT_MAJOR_VERSION, 5) {
    warning(" >>> You're trying to build with Qt 4")
    warning(" >>> This version of QtEmu requires Qt 5")
    warning(" >>> You might need to use qmake-qt5 instead $$escape_expand(\\n)")

    error("Aborting!")
}

# SOURCE_DATE_EPOCH is read from environment, to enable reproducible builds in Debian
source_date_epoch = $$(SOURCE_DATE_EPOCH)
!isEmpty(source_date_epoch) {
    message("Creating a reproducible build (avoiding hardcoded timestamps) \
             because SOURCE_DATE_EPOCH is defined: $$(SOURCE_DATE_EPOCH)")
    DEFINES += REPRODUCIBLEBUILD
}


TARGET = qtemu

TEMPLATE = app

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/helpwidget.cpp \
           src/aboutwidget.cpp \
           src/configwindow.cpp \
           src/machinewizard.cpp \
           src/machine.cpp \
           src/utils/systemutils.cpp \
           src/newmachine/generalpage.cpp \
           src/newmachine/hardwarepage.cpp \
           src/newmachine/acceleratorpage.cpp \
           src/newmachine/memorypage.cpp \
           src/newmachine/diskpage.cpp \
           src/newmachine/conclusionpage.cpp \
           src/machineconfig/machineconfigwindow.cpp \
           src/utils/logger.cpp \
           src/machineutils.cpp \
    src/machineconfig/machineconfiggeneral.cpp \
    src/machineconfig/machineconfighardware.cpp \
    src/machineconfig/machineconfigboot.cpp \
    src/machineconfig/machineconfigmedia.cpp \
    src/machineconfig/machineconfignetwork.cpp \
    src/machineconfig/machineconfigaudio.cpp \
    src/machineconfig/machineconfigaccel.cpp \
    src/utils/newdiskwizard.cpp

HEADERS  += src/mainwindow.h \
            src/helpwidget.h \
            src/aboutwidget.h \
            src/configwindow.h \
            src/machinewizard.h \
            src/machine.h \
            src/utils/systemutils.h \
            src/newmachine/generalpage.h \
            src/newmachine/hardwarepage.h \
            src/newmachine/acceleratorpage.h \
            src/newmachine/memorypage.h \
            src/newmachine/diskpage.h \
            src/newmachine/conclusionpage.h \
            src/machineconfig/machineconfigwindow.h \
            src/utils/logger.h \
            src/machineutils.h \
    src/machineconfig/machineconfiggeneral.h \
    src/machineconfig/machineconfighardware.h \
    src/machineconfig/machineconfigboot.h \
    src/machineconfig/machineconfigmedia.h \
    src/machineconfig/machineconfignetwork.h \
    src/machineconfig/machineconfigaudio.h \
    src/machineconfig/machineconfigaccel.h \
    src/utils/newdiskwizard.h

OTHER_FILES += \
    CHANGELOG \
    LICENSE \
    qtemu.desktop \
    README \
    translations
    
RESOURCES += qtemu.qrc



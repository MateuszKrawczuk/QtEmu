/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

// Qt
#include <QApplication>
#include <QTranslator>

// C++ standard library
#include <iostream>

// Local
#include "mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication qtemuApp(argc, argv);
    qtemuApp.setApplicationName("QtEmu");
    qtemuApp.setApplicationVersion("2.0-alpha");
    qtemuApp.setOrganizationName("QtEmu Developers");
    qtemuApp.setOrganizationDomain("https://www.qtemu.org");

    std::cout << QString("QtEmu v%1 # QtEmu Developers")
                        .arg(qtemuApp.applicationVersion()).toStdString();

    std::cout << QString("- Built with Qt v%1").arg(QT_VERSION_STR)
                                               .toStdString();
    //TODO make reproductible builds
    //TODO implement command line arguments
    //TODO implement translations
    //TODO put all todo in TODO file

    std::cout << "\n";

    std::cout << QString("- Running with Qt v%1\n\n").arg(qVersion())
                                                     .toStdString();
    std::cout.flush();

    MainWindow qtemuWindow;
    qtemuWindow.show();

    return qtemuApp.exec();

}

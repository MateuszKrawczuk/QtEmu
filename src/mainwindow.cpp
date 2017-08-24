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

// Local
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this -> setWindowTitle("QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/icon/32x32/qtemu.png")));
    this -> setMinimumSize(500, 600);

    // Close the application when all windows are closed
    qApp->setQuitOnLastWindowClosed(true);

    QSettings settings;

    // Create the menus
    createMenus();

}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow destroyed";
}

/*!
 * \brief Create the menus
 *
 * Create the menus for the top toolbar
 */
void MainWindow::createMenus() {

    // File
    fileMenu = new QMenu(tr("&File"), this);

    // Machine
    machineMenu = new QMenu(tr("&Machine"), this);

    // Help
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(helpManual);
    helpMenu->addSeparator();

    // Add the menus to the main menu bar
    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(machineMenu);
    this->menuBar()->addMenu(helpMenu);
}

/*!
 * \brief Create the actions for the menus
 *
 * Create the differents actions for the menus
 */
void MainWindow::createMenusActions() {

    // Actions for File menu

    // Actions for Machine menu

    // Actions for Help menu
    helpManual = new QAction(QIcon::fromTheme("qtemu", QIcon(":/icon/32x32/qtemu.png")),
                             tr("QtEmu &Help"),
                             this);
    connect(helpManual, SIGNAL(triggered()), helpManualWidget, SLOT(show()));
    helpManual->setShortcut(Qt::Key_F1);

}

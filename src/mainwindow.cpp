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

    helpwidget  = new HelpWidget(nullptr);
    aboutwidget = new AboutWidget(nullptr);

    // Create the menus
    createMenusActions();
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
    helpMenu -> addAction(helpQuickHelp);
    helpMenu -> addSeparator();
    helpMenu -> addAction(helpQtEmuWebsite);
    helpMenu -> addAction(helpQtEmuBugTracker);
    helpMenu -> addAction(helpQemuWebsite);
    helpMenu -> addSeparator();
    helpMenu -> addAction(helpAbout);

    // Add the menus to the main menu bar
    this->menuBar() -> addMenu(fileMenu);
    this->menuBar() -> addMenu(machineMenu);
    this->menuBar() -> addMenu(helpMenu);
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
    helpQuickHelp = new QAction(QIcon::fromTheme("help-contents",
                                                 QIcon(":/icon/32x32/qtemu.png")),
                                tr("QtEmu &Quick Help"),
                                this);
    helpQuickHelp -> setShortcut(Qt::Key_F1);
    connect(helpQuickHelp, &QAction::triggered,
            helpwidget, &QWidget::show);

    helpQtEmuWebsite = new QAction(QIcon::fromTheme("internet-web-browser",
                                                    QIcon(":/icon/32x32/qtemu.png")),
                                   tr("QtEmu &Website"),
                                   this);
    connect(helpQtEmuWebsite, &QAction::triggered,
            this, &MainWindow::visitQtEmuWebsite);

    helpQtEmuBugTracker = new QAction(QIcon::fromTheme("tools-report-bug",
                                                       QIcon(":/icon/32x32/qtemu.png")),
                                      tr("QtEmu Bug Tracker"),
                                      this);
    connect(helpQtEmuBugTracker, &QAction::triggered,
            this, &MainWindow::visitQtEmuBugTracker);

    helpQemuWebsite = new QAction(QIcon::fromTheme("internet-web-browser",
                                                   QIcon(":/icon/32x32/qtemu.png")),
                                  tr("Qemu &Website"),
                                  this);
    connect(helpQemuWebsite, &QAction::triggered,
            this, &MainWindow::visitQemuWebsite);

    helpAbout = new QAction(QIcon::fromTheme("qtemu",
                                             QIcon(":/icon/32x32/qtemu.png")),
                            tr("&About QtEmu"),
                            this);
    connect(helpAbout, &QAction::triggered,
            aboutwidget, &QWidget::show);
}

/*!
 * \brief Open QtEmu Website
 *
 * Open the QtEmu official webpage into your browser
 */
void MainWindow::visitQtEmuWebsite() {
    QDesktopServices::openUrl(QUrl("https://www.qtemu.org"));
}

/*!
 * \brief Open QtEmu Bug Tracker
 *
 * Open the QtEmu Bug Tracker into your browser
 */
void MainWindow::visitQtEmuBugTracker() {
    QDesktopServices::openUrl(QUrl("https://gitlab.com/carlavilla/Qtemu/issues"));
}

/*!
 * \brief Open Qemu Website
 *
 * Open the Qemu official webpage into your browser
 */
void MainWindow::visitQemuWebsite() {
    QDesktopServices::openUrl(QUrl("https://www.qemu.org/"));
}

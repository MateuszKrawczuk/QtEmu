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
#include "machinewizard.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this -> setWindowTitle("QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/icon/32x32/qtemu.png")));
    this -> setMinimumSize(700, 500);

    // Close the application when all windows are closed
    qApp -> setQuitOnLastWindowClosed(true);

    QSettings settings;

    configWindow = new ConfigWindow(this);
    helpwidget  = new HelpWidget(this);
    aboutwidget = new AboutWidget(this);

    // Prepare main layout
    osListWidget = new QListWidget();
    osListWidget -> setViewMode(QListView::ListMode);
    osListWidget -> setIconSize(QSize(32, 32));
    osListWidget -> setMovement(QListView::Static);
    osListWidget -> setMaximumWidth(170);
    osListWidget -> setSpacing(7);

    osDetailsStackedWidget = new QStackedWidget(this);
    osDetailsStackedWidget -> setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::MinimumExpanding);

    containerLayout = new QHBoxLayout();
    containerLayout -> addWidget(osListWidget);
    containerLayout -> addWidget(osDetailsStackedWidget);

    mainLayout = new QVBoxLayout();
    mainLayout -> addLayout(containerLayout, 20);
    mainLayout -> addSpacing(8);
    mainLayout -> addStretch(1);

    mainWidget = new QWidget(this);

    mainWidget -> setLayout(mainLayout);

    this -> setCentralWidget(mainWidget);

    // Create the menus
    createMenusActions();
    createMenus();
    createToolBars();

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
    fileMenu -> addAction(preferencesApp);
    fileMenu -> addSeparator();
    fileMenu -> addAction(checkUpdateApp);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitApp);

    // Machine
    machineMenu = new QMenu(tr("&Machine"), this);
    machineMenu -> addAction(newMachine);
    machineMenu -> addAction(addMachine);
    machineMenu -> addAction(settingsMachine);
    machineMenu -> addAction(duplicateMachine);
    machineMenu -> addAction(removeMachine);
    machineMenu -> addAction(groupMachine);

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
    preferencesApp = new QAction(QIcon::fromTheme("configure",
                                                  QIcon(":/icon/32x32/qtemu.png")),
                                 tr("Preferences"),
                                 this);
    //preferencesApp -> setShortcut(Qt::Key_F1);
    connect(preferencesApp, &QAction::triggered,
            configWindow, &QWidget::show);

    checkUpdateApp = new QAction(QIcon::fromTheme("update-none",
                                                  QIcon(":/icon/32x32/qtemu.png")),
                                 tr("Check for updates"),
                                 this);

    exitApp = new QAction(QIcon::fromTheme("application-exit",
                                           QIcon(":/icon/32x32/qtemu.png")),
                          tr("Exit"),
                          this);

    // Actions for Machine menu

    newMachine = new QAction(QIcon::fromTheme("project-development-new-template",
                                              QIcon(":/icon/32x32/qtemu.png")),
                             tr("New Machine"),
                             this);
    connect(newMachine, &QAction::triggered,
            this, &MainWindow::createNewMachine);

    addMachine = new QAction(QIcon::fromTheme("project-development",
                                              QIcon(":/icon/32x32/qtemu.png")),
                             tr("Add Machine"),
                             this);

    settingsMachine = new QAction(QIcon::fromTheme("settings-configure",
                                                   QIcon(":/icon/32x32/qtemu.png")),
                                  tr("Machine Settings"),
                                  this);

    duplicateMachine = new QAction(QIcon::fromTheme("edit-duplicate",
                                                    QIcon(":/icon/32x32/qtemu.png")),
                                   tr("Duplicate Machine"),
                                   this);

    removeMachine = new QAction(QIcon::fromTheme("project-development-close",
                                                 QIcon(":/icon/32x32/qtemu.png")),
                                tr("Remove Machine"),
                                this);

    groupMachine = new QAction(QIcon::fromTheme("view-group",
                                                QIcon(":/icon/32x32/qtemu.png")),
                               tr("Group Machines"),
                               this);


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

void MainWindow::createToolBars() {
    this -> mainToolBar = addToolBar(tr("Toolbar"));

    mainToolBar -> setToolButtonStyle(Qt::ToolButtonFollowStyle);

    mainToolBar -> addAction(this -> newMachine);
    mainToolBar -> addAction(this -> settingsMachine);
    mainToolBar -> addSeparator();

    mainToolBar -> setMovable(false);

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

void MainWindow::createNewMachine() {

    machine = new Machine(this);

    MachineWizard newMachineWizard(machine, this);

    newMachineWizard.show();
    newMachineWizard.exec();

    qDebug() << "The new Machine";
    qDebug() << "Name: " << this -> machine -> getName();
    qDebug() << "Operating System type: " << this -> machine -> getOSType();
    qDebug() << "Operating System version: " << this -> machine -> getOSVersion();
    qDebug() << "CPU Type: " << this -> machine -> getCPUType();
    qDebug() << "CPU Count: " << this -> machine -> getCPUCount();
    qDebug() << "Cores per socket: " << this -> machine -> getCoresSocket();
    qDebug() << "Socket count: " << this -> machine -> getSocketCount();
    qDebug() << "Threads per core: " << this -> machine -> getThreadsCore();
    qDebug() << "Max hot CPU: " << this -> machine -> getMaxHotCPU();
    qDebug() << "Graphics: " << this -> machine -> getGPUType();
    qDebug() << "Keyboard: " << this -> machine -> getKeyboard();

}

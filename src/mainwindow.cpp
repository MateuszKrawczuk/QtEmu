/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
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
    osListWidget = new QListWidget(this);
    osListWidget -> setViewMode(QListView::ListMode);
    osListWidget -> setContextMenuPolicy(Qt::CustomContextMenu);
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
    loadMachines();

    this -> loadUI(osListWidget -> count());

    // Connect
    connect(osListWidget, &QListWidget::itemClicked,
            this, &MainWindow::changeMachine);

    connect(osListWidget, &QListWidget::customContextMenuRequested,
            this, &MainWindow::machinesMenu);
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
    fileMenu -> addAction(preferencesAppAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(checkUpdateAppAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitAppAction);

    // Machine
    machineMenu = new QMenu(tr("&Machine"), this);
    machineMenu -> addAction(newMachineAction);
    machineMenu -> addAction(settingsMachineAction);
    machineMenu -> addAction(duplicateMachineAction);
    machineMenu -> addAction(removeMachineAction);
    //machineMenu -> addAction(groupMachine);

    // Help
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu -> addAction(helpQuickHelpAction);
    helpMenu -> addSeparator();
    helpMenu -> addAction(helpQtEmuWebsiteAction);
    helpMenu -> addAction(helpQtEmuBugTrackerAction);
    helpMenu -> addAction(helpQemuWebsiteAction);
    helpMenu -> addSeparator();
    helpMenu -> addAction(helpAboutAction);

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
    preferencesAppAction = new QAction(QIcon::fromTheme("configure",
                                                        QIcon(":/icon/32x32/qtemu.png")),
                                       tr("Preferences"),
                                       this);
    //preferencesApp -> setShortcut(Qt::Key_F1);
    connect(preferencesAppAction, &QAction::triggered,
            configWindow, &QWidget::show);

    checkUpdateAppAction = new QAction(QIcon::fromTheme("update-none",
                                                        QIcon(":/icon/32x32/qtemu.png")),
                                       tr("Check for updates"),
                                       this);

    exitAppAction = new QAction(QIcon::fromTheme("application-exit",
                                                 QIcon(":/icon/32x32/qtemu.png")),
                                tr("Exit"),
                                this);

    // Actions for Machine menu

    newMachineAction = new QAction(QIcon::fromTheme("project-development-new-template",
                                              QIcon(":/icon/32x32/qtemu.png")),
                                   tr("New Machine"),
                                   this);
    connect(newMachineAction, &QAction::triggered,
            this, &MainWindow::createNewMachine);

    settingsMachineAction = new QAction(QIcon::fromTheme("settings-configure",
                                                         QIcon(":/icon/32x32/qtemu.png")),
                                        tr("Machine Settings"),
                                        this);

    duplicateMachineAction = new QAction(QIcon::fromTheme("edit-duplicate",
                                                          QIcon(":/icon/32x32/qtemu.png")),
                                         tr("Duplicate Machine"),
                                         this);

    removeMachineAction = new QAction(QIcon::fromTheme("project-development-close",
                                                       QIcon(":/icon/32x32/qtemu.png")),
                                      tr("Remove Machine"),
                                      this);
    connect(removeMachineAction, &QAction::triggered,
            this, &MainWindow::deleteMachine);

    /*groupMachine = new QAction(QIcon::fromTheme("view-group",
                                                QIcon(":/icon/32x32/qtemu.png")),
                               tr("Group Machines"),
                               this);
    */

    // Actions for Help menu
    helpQuickHelpAction = new QAction(QIcon::fromTheme("help-contents",
                                                       QIcon(":/icon/32x32/qtemu.png")),
                                      tr("QtEmu &Quick Help"),
                                      this);
    helpQuickHelpAction -> setShortcut(Qt::Key_F1);
    connect(helpQuickHelpAction, &QAction::triggered,
            helpwidget, &QWidget::show);

    helpQtEmuWebsiteAction = new QAction(QIcon::fromTheme("internet-web-browser",
                                                          QIcon(":/icon/32x32/qtemu.png")),
                                         tr("QtEmu &Website"),
                                         this);
    connect(helpQtEmuWebsiteAction, &QAction::triggered,
            this, &MainWindow::visitQtEmuWebsite);

    helpQtEmuBugTrackerAction = new QAction(QIcon::fromTheme("tools-report-bug",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                            tr("QtEmu Bug Tracker"),
                                            this);
    connect(helpQtEmuBugTrackerAction, &QAction::triggered,
            this, &MainWindow::visitQtEmuBugTracker);

    helpQemuWebsiteAction = new QAction(QIcon::fromTheme("internet-web-browser",
                                                         QIcon(":/icon/32x32/qtemu.png")),
                                        tr("Qemu &Website"),
                                        this);
    connect(helpQemuWebsiteAction, &QAction::triggered,
            this, &MainWindow::visitQemuWebsite);

    helpAboutAction = new QAction(QIcon::fromTheme("qtemu",
                                                   QIcon(":/icon/32x32/qtemu.png")),
                                  tr("&About QtEmu"),
                                  this);
    connect(helpAboutAction, &QAction::triggered,
            aboutwidget, &QWidget::show);

    // Actions for Machine toolbar
    startMachineAction = new QAction(this);
    startMachineAction -> setIcon(QIcon::fromTheme("kt-start",
                                                   QIcon(":/icon/32x32/qtemu.png")));
    startMachineAction -> setToolTip(tr("Start machine"));
    connect(startMachineAction, &QAction::triggered,
            this, &MainWindow::runMachine);

    stopMachineAction = new QAction(this);
    stopMachineAction -> setIcon(QIcon::fromTheme("kt-stop",
                                                  QIcon(":/icon/32x32/qtemu.png")));
    stopMachineAction -> setToolTip(tr("Stop machine"));

    resetMachineAction = new QAction(this);
    resetMachineAction -> setIcon(QIcon::fromTheme("chronometer-reset",
                                                   QIcon(":/icon/32x32/qtemu.png")));
    resetMachineAction -> setToolTip(tr("Reset machine"));

    pauseMachineAction = new QAction(this);
    pauseMachineAction -> setIcon(QIcon::fromTheme("kt-pause",
                                                   QIcon(":/icon/32x32/qtemu.png")));
    pauseMachineAction -> setToolTip(tr("Pause machine"));

    saveMachineAction = new QAction(this);
    saveMachineAction -> setIcon(QIcon::fromTheme("system-save-session",
                                                  QIcon(":/icon/32x32/qtemu.png")));
    saveMachineAction -> setToolTip(tr("Save machine"));

}

/**
 * @brief Create the toolbar
 *
 * Create the main toolbar of the app
 */
void MainWindow::createToolBars() {
    this -> mainToolBar = addToolBar(tr("Toolbar"));

    mainToolBar -> setToolButtonStyle(Qt::ToolButtonFollowStyle);

    mainToolBar -> addAction(this -> newMachineAction);
    mainToolBar -> addAction(this -> settingsMachineAction);
    mainToolBar -> addSeparator();
    mainToolBar -> addAction(this -> startMachineAction);
    mainToolBar -> addAction(this -> stopMachineAction);
    mainToolBar -> addAction(this -> resetMachineAction);
    mainToolBar -> addAction(this -> pauseMachineAction);
    mainToolBar -> addAction(this -> saveMachineAction);

    mainToolBar -> setMovable(false);

}

/*!
 * \brief Load created machines
 *
 * Load all the machines stored in the qtemu.json file on config data folder
 */
void MainWindow::loadMachines() {

    QSettings settings;
    settings.beginGroup("DataFolder");

    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all data included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonArray machines = machinesDocument["machines"].toArray();

    for (int i = 0; i < machines.size(); ++i) {
        QJsonObject machineJSON = machines[i].toObject();

        QListWidgetItem *machine = new QListWidgetItem(machineJSON["name"].toString(), this -> osListWidget);
        machine -> setData(QMetaType::QUuid, machineJSON["uuid"].toString());
        // TODO: Check if the json it's incomplete and the image not exits
        machine -> setIcon(QIcon(":/images/os/64x64/" +
                                 SystemUtils::getOsIcon(machineJSON["icon"].toString())));
    }


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

/*!
 * \brief Open the create machine wizard
 *
 * Open the machine wizard to create a new machine
 */
void MainWindow::createNewMachine() {

    machine = new Machine(this);

    machine -> addAudio("ac97", "Intel AC97(82801AA)");
    machine -> setSocketCount(0);
    machine -> setCoresSocket(0);
    machine -> setThreadsCore(0);
    machine -> setMaxHotCPU(0);

    MachineWizard newMachineWizard(machine, this -> osListWidget, this);

    newMachineWizard.show();
    newMachineWizard.exec();

    this -> loadUI(this -> osListWidget -> count());

}

/*!
 * \brief Start the selected machine
 *
 * Read the machine config file,
 * make the command and start the selected machine
 */
void MainWindow::runMachine() {
    QUuid machineUuid = this -> osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    QStringList args = MachineUtils::generateMachineCommand(machineUuid);

    QProcess *machineProcess = new QProcess();

    QString program;

    #ifdef Q_OS_LINUX
    program = "qemu-system-x86_64";
    #endif

    machineProcess -> start(program, args);

    // TODO: Control the output of the machineProcess

    // Connections


}

/*!
  * \brief Delete the selected machine
  *
  * Delete the selected machine and its associated files
  */
void MainWindow::deleteMachine() {
    QUuid machineUuid = this -> osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    bool isMachineDeleted = MachineUtils::deleteMachine(machineUuid);

    if (isMachineDeleted) {
        this -> osListWidget -> takeItem(this -> osListWidget -> currentRow());
        this -> loadUI(this -> osListWidget -> count());
    }

}

/*!
 * \brief Enable/Disable buttons
 *
 * Enable/Disable the buttons in the menubar and in the main ui
 * If the osListWidget item doesn't have elements, elements
 * related to the VM actions are disabled. If the osListWidget have
 * at least one element, elements are enabled
 */
void MainWindow::loadUI(const int itemCount) {

    this -> stopMachineAction -> setEnabled(false);
    this -> resetMachineAction -> setEnabled(false);
    this -> pauseMachineAction -> setEnabled(false);
    this -> saveMachineAction -> setEnabled(false);

    if (itemCount == 0) {
        this -> settingsMachineAction  -> setEnabled(false);
        this -> duplicateMachineAction -> setEnabled(false);
        this -> removeMachineAction    -> setEnabled(false);

        this -> startMachineAction     -> setEnabled(false);
    } else {
        this -> osListWidget -> setCurrentRow(0);

        this -> settingsMachineAction  -> setEnabled(true);
        this -> duplicateMachineAction -> setEnabled(true);
        this -> removeMachineAction    -> setEnabled(true);

        this -> startMachineAction     -> setEnabled(true);
    }

}

/**
 * @brief Enable or disable the machine action items
 * @param machineItem, machine item of the list
 *
 * Enable/Disable the machine action items depending the
 * state of the machine
 */
void MainWindow::changeMachine(QListWidgetItem *machineItem) {
    qDebug() << "Change machine" << machineItem -> data(QMetaType::QUuid).toUuid();
}

void MainWindow::machinesMenu(const QPoint &pos) {
    qDebug() << "Machine menu, pos: " << pos;
}

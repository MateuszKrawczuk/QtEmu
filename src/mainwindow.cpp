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
    this -> setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    this -> setMinimumSize(700, 500);

    // Close the application when all windows are closed
    qApp -> setQuitOnLastWindowClosed(true);

    QSettings settings;

    m_configWindow = new ConfigWindow(this);
    m_helpwidget  = new HelpWidget(this);
    m_aboutwidget = new AboutWidget(this);

    // Prepare main layout
    m_osListWidget = new QListWidget(this);
    m_osListWidget -> setViewMode(QListView::ListMode);
    m_osListWidget -> setContextMenuPolicy(Qt::CustomContextMenu);
    m_osListWidget -> setIconSize(QSize(32, 32));
    m_osListWidget -> setMovement(QListView::Static);
    m_osListWidget -> setMaximumWidth(170);
    m_osListWidget -> setSpacing(7);

    m_machineNameLabel     = new QLabel();
    m_machineOsLabel       = new QLabel();
    m_machineCPULabel      = new QLabel();
    m_machineRAMLabel      = new QLabel();
    m_machineGraphicsLabel = new QLabel();
    m_machineAudioLabel    = new QLabel();
    m_machineAccelLabel    = new QLabel();
    m_machineDiskLabel     = new QLabel();
    m_machineNetworkLabel  = new QLabel();

    m_machineDetailsLayout = new QFormLayout();
    m_machineDetailsLayout -> setSpacing(7);
    m_machineDetailsLayout -> setHorizontalSpacing(50);
    m_machineDetailsLayout -> setLabelAlignment(Qt::AlignLeft);
    m_machineDetailsLayout -> setContentsMargins(10, 20, 0, 0);
    m_machineDetailsLayout -> addRow(tr("Name") + ":", m_machineNameLabel);
    m_machineDetailsLayout -> addRow(tr("Operating System") + ":", m_machineOsLabel);
    m_machineDetailsLayout -> addRow(tr("CPU") + ":", m_machineCPULabel);
    m_machineDetailsLayout -> addRow(tr("RAM") + ":", m_machineRAMLabel);
    m_machineDetailsLayout -> addRow(tr("Graphics") + ":", m_machineGraphicsLabel);
    m_machineDetailsLayout -> addRow(tr("Audio") + ":", m_machineAudioLabel);
    m_machineDetailsLayout -> addRow(tr("Accelerator") + ":", m_machineAccelLabel);
    m_machineDetailsLayout -> addRow(tr("Disk") + ":", m_machineDiskLabel);
    m_machineDetailsLayout -> addRow(tr("Network") + ":", m_machineNetworkLabel);

    m_machineDetailsGroup = new QGroupBox(tr("Machine details"));
    m_machineDetailsGroup -> setAlignment(Qt::AlignHCenter);
    m_machineDetailsGroup -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_machineDetailsGroup -> setLayout(m_machineDetailsLayout);

    m_osDetailsStackedWidget = new QStackedWidget(this);
    m_osDetailsStackedWidget -> setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    m_osDetailsStackedWidget -> addWidget(m_machineDetailsGroup);

    m_containerLayout = new QHBoxLayout();
    m_containerLayout -> addWidget(m_osListWidget);
    m_containerLayout -> addWidget(m_osDetailsStackedWidget);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout -> addLayout(m_containerLayout, 20);
    m_mainLayout -> addSpacing(8);
    m_mainLayout -> addStretch(1);

    m_mainWidget = new QWidget(this);

    m_mainWidget -> setLayout(m_mainLayout);

    this -> setCentralWidget(m_mainWidget);

    // Create the menus
    createMenusActions();
    createMenus();
    createToolBars();
    loadMachines();

    this -> loadUI(m_osListWidget -> count());

    // Connect
    connect(m_osListWidget, &QListWidget::itemClicked,
            this, &MainWindow::changeMachine);

    connect(m_osListWidget, &QListWidget::customContextMenuRequested,
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
    m_fileMenu = new QMenu(tr("&File"), this);
    m_fileMenu -> addAction(m_preferencesAppAction);
    m_fileMenu -> addSeparator();
    m_fileMenu -> addAction(m_checkUpdateAppAction);
    m_fileMenu -> addSeparator();
    m_fileMenu -> addAction(m_exitAppAction);

    // Machine
    m_machineMenu = new QMenu(tr("&Machine"), this);
    m_machineMenu -> addAction(m_newMachineAction);
    m_machineMenu -> addAction(m_settingsMachineAction);
    m_machineMenu -> addAction(m_duplicateMachineAction);
    m_machineMenu -> addAction(m_removeMachineAction);
    //machineMenu -> addAction(groupMachine);

    // Help
    m_helpMenu = new QMenu(tr("&Help"), this);
    m_helpMenu -> addAction(m_helpQuickHelpAction);
    m_helpMenu -> addSeparator();
    m_helpMenu -> addAction(m_helpQtEmuWebsiteAction);
    m_helpMenu -> addAction(m_helpQtEmuBugTrackerAction);
    m_helpMenu -> addAction(m_helpQemuWebsiteAction);
    m_helpMenu -> addSeparator();
    m_helpMenu -> addAction(m_helpAboutAction);

    // Add the menus to the main menu bar
    this -> menuBar() -> addMenu(m_fileMenu);
    this -> menuBar() -> addMenu(m_machineMenu);
    this -> menuBar() -> addMenu(m_helpMenu);
}

/*!
 * \brief Create the actions for the menus
 *
 * Create the differents actions for the menus
 */
void MainWindow::createMenusActions() {

    // Actions for File menu
    m_preferencesAppAction = new QAction(QIcon::fromTheme("configure",
                                                          QIcon(QPixmap(":/images/icons/breeze/32x32/configure.svg"))),
                                         tr("Preferences"),
                                         this);
    //preferencesApp -> setShortcut(Qt::Key_F1);
    connect(m_preferencesAppAction, &QAction::triggered,
            m_configWindow, &QWidget::show);

    m_checkUpdateAppAction = new QAction(QIcon::fromTheme("update-none",
                                                          QIcon(QPixmap(":/images/icons/breeze/32x32/update-none.svg"))),
                                         tr("Check for updates"),
                                         this);

    m_exitAppAction = new QAction(QIcon::fromTheme("application-exit",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/applcation-exit.svg"))),
                                  tr("Exit"),
                                  this);

    // Actions for Machine menu

    m_newMachineAction = new QAction(QIcon::fromTheme("project-development-new-template",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/project-development-new-template.svg"))),
                                     tr("New Machine"),
                                     this);
    connect(m_newMachineAction, &QAction::triggered,
            this, &MainWindow::createNewMachine);

    m_settingsMachineAction = new QAction(QIcon::fromTheme("settings-configure",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/settings-configure.svg"))),
                                          tr("Machine Settings"),
                                          this);

    m_duplicateMachineAction = new QAction(QIcon::fromTheme("edit-duplicate",
                                                            QIcon(QPixmap(":/images/icons/breeze/32x32/edit-duplicate.svg"))),
                                           tr("Duplicate Machine"),
                                           this);

    m_removeMachineAction = new QAction(QIcon::fromTheme("project-development-close",
                                                         QIcon(QPixmap(":/images/icons/breeze/32x32/project-development-close.svg"))),
                                        tr("Remove Machine"),
                                        this);
    connect(m_removeMachineAction, &QAction::triggered,
            this, &MainWindow::deleteMachine);

    /*groupMachine = new QAction(QIcon::fromTheme("view-group",
                                                QIcon(":/icon/32x32/qtemu.png")),
                               tr("Group Machines"),
                               this);
    */

    // Actions for Help menu
    m_helpQuickHelpAction = new QAction(QIcon::fromTheme("help-contents",
                                                         QIcon(QPixmap(":/images/icons/breeze/32x32/help-contents.svg"))),
                                        tr("QtEmu &Quick Help"),
                                        this);
    m_helpQuickHelpAction -> setShortcut(Qt::Key_F1);
    connect(m_helpQuickHelpAction, &QAction::triggered,
            m_helpwidget, &QWidget::show);

    m_helpQtEmuWebsiteAction = new QAction(QIcon::fromTheme("internet-web-browser",
                                                            QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))),
                                           tr("QtEmu &Website"),
                                           this);
    connect(m_helpQtEmuWebsiteAction, &QAction::triggered,
            this, &MainWindow::visitQtEmuWebsite);

    m_helpQtEmuBugTrackerAction = new QAction(QIcon::fromTheme("tools-report-bug",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/tools-report-bug.svg"))),
                                              tr("QtEmu Bug Tracker"),
                                              this);
    connect(m_helpQtEmuBugTrackerAction, &QAction::triggered,
            this, &MainWindow::visitQtEmuBugTracker);

    m_helpQemuWebsiteAction = new QAction(QIcon::fromTheme("internet-web-browser",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))),
                                          tr("Qemu &Website"),
                                          this);
    connect(m_helpQemuWebsiteAction, &QAction::triggered,
            this, &MainWindow::visitQemuWebsite);

    m_helpAboutAction = new QAction(QIcon::fromTheme("qtemu",
                                                     QIcon(":/images/qtemu.png")),
                                    tr("&About QtEmu"),
                                    this);
    connect(m_helpAboutAction, &QAction::triggered,
            m_aboutwidget, &QWidget::show);

    // Actions for Machine toolbar
    m_startMachineAction = new QAction(this);
    m_startMachineAction -> setIcon(QIcon::fromTheme("kt-start",
                                                     QIcon(":/icon/32x32/qtemu.png")));
    m_startMachineAction -> setToolTip(tr("Start machine"));
    connect(m_startMachineAction, &QAction::triggered,
            this, &MainWindow::runMachine);

    m_stopMachineAction = new QAction(this);
    m_stopMachineAction -> setIcon(QIcon::fromTheme("kt-stop",
                                                    QIcon(":/icon/32x32/qtemu.png")));
    m_stopMachineAction -> setToolTip(tr("Stop machine"));

    m_resetMachineAction = new QAction(this);
    m_resetMachineAction -> setIcon(QIcon::fromTheme("chronometer-reset",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/chronometer-reset.svg"))));
    m_resetMachineAction -> setToolTip(tr("Reset machine"));
    connect(m_resetMachineAction, &QAction::triggered,
            this, &MainWindow::resetMachine);

    m_pauseMachineAction = new QAction(this);
    m_pauseMachineAction -> setIcon(QIcon::fromTheme("kt-pause",
                                                     QIcon(":/icon/32x32/qtemu.png")));
    m_pauseMachineAction -> setToolTip(tr("Pause machine"));
    connect(m_pauseMachineAction, &QAction::triggered,
            this, &MainWindow::pauseMachine);

    /*m_saveMachineAction = new QAction(this);
    m_saveMachineAction -> setIcon(QIcon::fromTheme("system-save-session",
                                                    QIcon(":/icon/32x32/qtemu.png")));
    m_saveMachineAction -> setToolTip(tr("Save machine"));*/

}

/**
 * @brief Create the toolbar
 *
 * Create the main toolbar of the app
 */
void MainWindow::createToolBars() {
    this -> m_mainToolBar = addToolBar(tr("Toolbar"));

    m_mainToolBar -> setToolButtonStyle(Qt::ToolButtonFollowStyle);

    m_mainToolBar -> addAction(this -> m_newMachineAction);
    m_mainToolBar -> addAction(this -> m_settingsMachineAction);
    m_mainToolBar -> addSeparator();
    m_mainToolBar -> addAction(this -> m_startMachineAction);
    m_mainToolBar -> addAction(this -> m_stopMachineAction);
    m_mainToolBar -> addAction(this -> m_resetMachineAction);
    m_mainToolBar -> addAction(this -> m_pauseMachineAction);
    //m_mainToolBar -> addAction(this -> m_saveMachineAction);

    m_mainToolBar -> setMovable(false);

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

        QListWidgetItem *machine = new QListWidgetItem(machineJSON["name"].toString(), this -> m_osListWidget);
        machine -> setData(QMetaType::QUuid, machineJSON["uuid"].toString());
        // TODO: Check if the json it's incomplete and the image not exits
        machine -> setIcon(QIcon(":/images/os/64x64/" +
                                 SystemUtils::getOsIcon(machineJSON["icon"].toString())));

        QUuid machineUuid = machine -> data(QMetaType::QUuid).toUuid();
        this -> m_machinesList.append(generateMachineObject(machineUuid));
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

    m_machine = new Machine(this);

    m_machine -> addAudio("ac97", "Intel AC97(82801AA)");
    m_machine -> setSocketCount(0);
    m_machine -> setCoresSocket(0);
    m_machine -> setThreadsCore(0);
    m_machine -> setMaxHotCPU(0);

    MachineWizard newMachineWizard(m_machine, this -> m_osListWidget, this);

    newMachineWizard.show();
    newMachineWizard.exec();

    this -> loadUI(this -> m_osListWidget -> count());

}

/*!
 * \brief Start the selected machine
 *
 * Start the selected machine
 */
void MainWindow::runMachine() {
    QUuid machineUuid = this -> m_osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    foreach (Machine *machine, this -> m_machinesList) {
        if (machine -> getUuid() == machineUuid.toString()){
            machine -> runMachine(machineUuid);
        }
    }
}

/*!
 * \brief Reset the selected machine
 *
 * Reset the selected machine
 */
void MainWindow::resetMachine() {
    QUuid machineUuid = this -> m_osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    foreach (Machine *machine, this -> m_machinesList) {
        if (machine -> getUuid() == machineUuid.toString()){
            machine -> resetMachine();
        }
    }
}

/*!
 * \brief Pause or continue the selected machine
 *
 * If the State of the machine is Started, then
 * pause the machine.
 * If the State of the machine is Paused, then
 * continue the execution of the machine
 */
void MainWindow::pauseMachine() {
    QUuid machineUuid = this -> m_osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    foreach (Machine *machine, this -> m_machinesList) {
        if (machine -> getUuid() == machineUuid.toString()){
            machine -> pauseMachine();
        }
    }

}

/*!
  * \brief Delete the selected machine
  *
  * Delete the selected machine and its associated files
  */
void MainWindow::deleteMachine() {
    QUuid machineUuid = this -> m_osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    bool isMachineDeleted = MachineUtils::deleteMachine(machineUuid);

    if (isMachineDeleted) {
        this -> m_osListWidget -> takeItem(this -> m_osListWidget -> currentRow());
        this -> loadUI(this -> m_osListWidget -> count());
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

    this -> m_stopMachineAction  -> setEnabled(false);
    this -> m_resetMachineAction -> setEnabled(false);
    this -> m_pauseMachineAction -> setEnabled(false);
    //this -> m_saveMachineAction  -> setEnabled(false);

    if (itemCount == 0) {
        this -> m_settingsMachineAction  -> setEnabled(false);
        this -> m_duplicateMachineAction -> setEnabled(false);
        this -> m_removeMachineAction    -> setEnabled(false);

        this -> m_startMachineAction     -> setEnabled(false);
    } else {
        this -> m_osListWidget -> setCurrentRow(0);

        this -> m_settingsMachineAction  -> setEnabled(true);
        this -> m_duplicateMachineAction -> setEnabled(true);
        this -> m_removeMachineAction    -> setEnabled(true);

        this -> m_startMachineAction     -> setEnabled(true);
    }

    // Show the machine data in the labels
    if (this -> m_machinesList.size() > 0) {
        fillMachineDetailsSection(this -> m_machinesList[0]);
    }

}

/**
 * @brief Generate the machine object for the list
 * @param machineJSON, JSON with the machine params
 * @return machine object
 *
 * Generate the machine object for the list
 */
Machine* MainWindow::generateMachineObject(const QUuid machineUuid) {

    QJsonObject machineJSON = MachineUtils::getMachineJsonObject(machineUuid);

    Machine *machine = new Machine(this);

    machine -> setState(Machine::Stopped);
    machine -> setName(machineJSON["name"].toString());
    machine -> setOSType(machineJSON["OSType"].toString());
    machine -> setOSVersion(machineJSON["OSVersion"].toString());
    machine -> setRAM(machineJSON["RAM"].toDouble());
    machine -> setUseNetwork(machineJSON["network"].toBool());
    machine -> setPath(machineJSON["path"].toString());
    machine -> setUuid(machineJSON["uuid"].toString());

    connect(machine, &Machine::machineStateChangedSignal,
            this, &MainWindow::machineStateChanged);

    return machine;
}

/**
 * @brief Enable or disable the machine action items
 * @param machineItem, machine item of the list
 *
 * Enable/Disable the machine action items depending the
 * state of the machine
 */
void MainWindow::changeMachine(QListWidgetItem *machineItem) {
    QUuid machineUuid = machineItem -> data(QMetaType::QUuid).toUuid();

    foreach (Machine *machine, this -> m_machinesList) {
        if (machine -> getUuid() == machineUuid.toString()) {
            controlMachineActions(machine -> getState());
            fillMachineDetailsSection(machine);
        }
    }

}

/**
 * @brief Fill the machine details section of the main UI
 * @param machine, machine with all the data
 *
 * Fill the machine details section of the main UI
 * with the machine selected in the m_osListWidget
 */
void MainWindow::fillMachineDetailsSection(Machine *machine){
    this -> m_machineNameLabel -> setText(machine -> getName());
    this -> m_machineOsLabel   -> setText(machine -> getOSType() + " - " + machine -> getOSVersion());

    this -> m_machineRAMLabel  -> setText(QString::number(machine -> getRAM()) + " MiB");
}

/**
 * @brief Show a machine's menu
 * @param pos, position
 *
 * Show a machine's menu when the user press
 * the second button of the mouse or touchpad
 */
void MainWindow::machinesMenu(const QPoint &pos) {
    qDebug() << "Machine menu, pos: " << pos;
}

/**
 * @brief Control when the state of a VM changes
 * @param newState, new state of the VM
 *
 * Control when the state of a VM changes
 */
void MainWindow::machineStateChanged(Machine::States newState) {
    controlMachineActions(newState);
}

/**
 * @brief Control if the state of the actions of the VM
 * @param state, state of the VM
 *
 * Control if the actions of the machines are enabled or disabled
 * based on the state of the VM
 */
void MainWindow::controlMachineActions(Machine::States state) {

    if (state == Machine::Started) {
        this -> m_startMachineAction -> setEnabled(false);
        this -> m_stopMachineAction  -> setEnabled(true);
        this -> m_resetMachineAction -> setEnabled(true);
        this -> m_pauseMachineAction -> setEnabled(true);
        //this -> m_saveMachineAction  -> setEnabled(true);
    } else if(state == Machine::Stopped) {
        this -> m_startMachineAction -> setEnabled(true);
        this -> m_stopMachineAction  -> setEnabled(false);
        this -> m_resetMachineAction -> setEnabled(false);
        this -> m_pauseMachineAction -> setEnabled(false);
        //this -> m_saveMachineAction  -> setEnabled(false);
    } else if(state == Machine::Paused) {
        this -> m_startMachineAction -> setEnabled(false);
        this -> m_stopMachineAction  -> setEnabled(false);
        this -> m_resetMachineAction -> setEnabled(false);
        this -> m_pauseMachineAction -> setEnabled(true);
        //this -> m_saveMachineAction  -> setEnabled(false);
    }
}

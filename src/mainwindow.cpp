// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "mainwindow.h"

/**
 * @brief The main window of the application
 * @param parent, parent widget
 *
 * Main window of QtEmu, with all the machines, menus, details of the machines...
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("QtEmu");
    this->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    this->setMinimumSize(700, 500);

    // Close the application when all windows are closed
    qApp->setQuitOnLastWindowClosed(true);

    QSettings settings;

    // Generate QEMU object
    qemuGlobalObject = new QEMU(this);

    m_configWindow = new ConfigWindow(qemuGlobalObject, this);
    m_helpwidget  = new HelpWidget(this);
    m_aboutwidget = new AboutWidget(this);

    // Prepare main layout
    m_osListWidget = new QListWidget(this);
    m_osListWidget->setViewMode(QListView::ListMode);
    m_osListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_osListWidget->setIconSize(QSize(32, 32));
    m_osListWidget->setMovement(QListView::Static);
    m_osListWidget->setMaximumWidth(170);
    m_osListWidget->setSpacing(7);

    m_machineNameLabel     = new QLabel(this);
    m_machineOsLabel       = new QLabel(this);
    m_machineCPULabel      = new QLabel(this);
    m_machineRAMLabel      = new QLabel(this);
    m_machineGraphicsLabel = new QLabel(this);
    m_machineAudioLabel    = new QLabel(this);
    m_machineAudioLabel->setWordWrap(true);
    m_machineAccelLabel    = new QLabel(this);
    m_machineNetworkLabel  = new QLabel(this);
    m_machineMediaLabel    = new QLabel(this);
    m_machineMediaLabel->setWordWrap(true);

    m_machineDetailsLayout = new QFormLayout();
    m_machineDetailsLayout->setSpacing(7);
    m_machineDetailsLayout->setHorizontalSpacing(50);
    m_machineDetailsLayout->setLabelAlignment(Qt::AlignLeft);
    m_machineDetailsLayout->setContentsMargins(10, 20, 0, 0);
    m_machineDetailsLayout->addRow(tr("Name") + ":", m_machineNameLabel);
    m_machineDetailsLayout->addRow(tr("Operating System") + ":", m_machineOsLabel);
    m_machineDetailsLayout->addRow(tr("CPU") + ":", m_machineCPULabel);
    m_machineDetailsLayout->addRow(tr("RAM") + ":", m_machineRAMLabel);
    m_machineDetailsLayout->addRow(tr("Graphics") + ":", m_machineGraphicsLabel);
    m_machineDetailsLayout->addRow(tr("Audio") + ":", m_machineAudioLabel);
    m_machineDetailsLayout->addRow(tr("Accelerator") + ":", m_machineAccelLabel);
    m_machineDetailsLayout->addRow(tr("Network") + ":", m_machineNetworkLabel);
    m_machineDetailsLayout->addRow(tr("Media") + ":", m_machineMediaLabel);

    m_machineDetailsGroup = new QGroupBox(tr("Machine details"), this);
    m_machineDetailsGroup->setAlignment(Qt::AlignHCenter);
    m_machineDetailsGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_machineDetailsGroup->setLayout(m_machineDetailsLayout);

    m_osDetailsStackedWidget = new QStackedWidget(this);
    m_osDetailsStackedWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    m_osDetailsStackedWidget->addWidget(m_machineDetailsGroup);

    m_containerLayout = new QHBoxLayout();
    m_containerLayout->addWidget(m_osListWidget);
    m_containerLayout->addWidget(m_osDetailsStackedWidget);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addLayout(m_containerLayout, 20);
    m_mainLayout->addSpacing(8);
    m_mainLayout->addStretch(1);

    m_mainWidget = new QWidget(this);

    m_mainWidget->setLayout(m_mainLayout);

    this->setCentralWidget(m_mainWidget);

    // Create the menus
    this->createMenusActions();
    this->createMenus();
    this->createToolBars();

    // Load all the machines
    this->m_osListWidget->setCurrentRow(0);
    this->loadMachines();
    this->loadUI(m_osListWidget->count());

    // Connect
    connect(m_osListWidget, &QListWidget::itemClicked,
            this, &MainWindow::changeMachine);

    connect(m_osListWidget, &QListWidget::customContextMenuRequested,
            this, &MainWindow::machinesMenu);


}
MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destroyed";
}
/**
 * @brief Create the menus
 *
 * Create the menus for the top toolbar
 */
void MainWindow::createMenus()
{
    // File
    m_fileMenu = new QMenu(tr("&File"), this);
    m_fileMenu->addAction(m_importMachineAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_preferencesAppAction);
#ifndef Q_OS_WIN
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_checkUpdateAppAction);
#endif
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAppAction);

    // Machine
    m_machineMenu = new QMenu(tr("&Machine"), this);
    m_machineMenu->addAction(m_newMachineAction);
    m_machineMenu->addAction(m_settingsMachineAction);
    m_machineMenu->addAction(m_exportMachineAction);
    m_machineMenu->addAction(m_removeMachineAction);
    m_killMachineAction = new QAction(QIcon::fromTheme("process-stop",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/process-stop.svg"))),
                                  tr("Kill machine"),
                                  this);
    m_killMachineAction->setToolTip(tr("Force stop selected virutal machine"));
    connect(m_killMachineAction, &QAction::triggered,
            this, &MainWindow::killMachine);
    m_machineMenu->addAction(m_killMachineAction);

    // Help
    m_helpMenu = new QMenu(tr("&Help"), this);
    m_helpMenu->addAction(m_helpQuickHelpAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_helpQtEmuWebsiteAction);
    m_helpMenu->addAction(m_helpQtEmuBugTrackerAction);
    m_helpMenu->addAction(m_helpQemuWebsiteAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_helpAboutAction);

    // Add the menus to the main menu bar
    this->menuBar()->addMenu(m_fileMenu);
    this->menuBar()->addMenu(m_machineMenu);
    this->menuBar()->addMenu(m_helpMenu);
}

void MainWindow::killMachine()
{
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            machine->killMachine();
            break;
        }
    }
}

/**
 * @brief Create the actions for the menus
 *
 * Create the differents actions for the menus
 */
void MainWindow::createMenusActions()
{
    // Actions for File menu
    m_importMachineAction = new QAction(QIcon::fromTheme("document-import",
                                                         QIcon(QPixmap(":/images/icons/breeze/32x32/document-import.svg"))),
                                         tr("Import machine"),
                                         this);
    connect(m_importMachineAction, &QAction::triggered,
            this, &MainWindow::importMachine);

    m_preferencesAppAction = new QAction(QIcon::fromTheme("configure",
                                                          QIcon(QPixmap(":/images/icons/breeze/32x32/configure.svg"))),
                                         tr("Preferences"),
                                         this);
    connect(m_preferencesAppAction, &QAction::triggered,
            m_configWindow, &QWidget::show);
#ifndef Q_OS_WIN
    m_checkUpdateAppAction = new QAction(QIcon::fromTheme("update-none",
                                                          QIcon(QPixmap(":/images/icons/breeze/32x32/update-none.svg"))),
                                         tr("Check for updates"),
                                         this);
    connect(m_checkUpdateAppAction, &QAction::triggered,
            this, &MainWindow::checkVersions);
#endif
    m_exitAppAction = new QAction(QIcon::fromTheme("application-exit",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/application-exit.svg"))),
                                  tr("Exit"),
                                  this);
    connect(m_exitAppAction, &QAction::triggered,
            this, &MainWindow::quitApp);

    // Actions for Machine menu
    m_newMachineAction = new QAction(QIcon::fromTheme("project-development-new-template",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/project-development-new-template.svg"))),
                                     tr("New Machine"),
                                     this);
    connect(m_newMachineAction, &QAction::triggered,
            this, &MainWindow::createNewMachine);

    m_settingsMachineAction = new QAction(QIcon::fromTheme("configure",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/settings-configure.svg"))),
                                          tr("Machine Settings"),
                                          this);
    connect(m_settingsMachineAction, &QAction::triggered,
            this, &MainWindow::machineOptions);

    m_exportMachineAction = new QAction(QIcon::fromTheme("document-export",
                                                         QIcon(QPixmap(":/images/icons/breeze/32x32/document-export.svg"))),
                                        tr("Export machine"),
                                        this);
    connect(m_exportMachineAction, &QAction::triggered,
            this, &MainWindow::exportMachine);

    m_removeMachineAction = new QAction(QIcon::fromTheme("project-development-close",
                                                         QIcon(QPixmap(":/images/icons/breeze/32x32/project-development-close.svg"))),
                                        tr("Remove Machine"),
                                        this);
    connect(m_removeMachineAction, &QAction::triggered,
            this, &MainWindow::deleteMachine);

    // Actions for Help menu
    m_helpQuickHelpAction = new QAction(QIcon::fromTheme("help-contents",
                                                         QIcon(QPixmap(":/images/icons/breeze/32x32/help-contents.svg"))),
                                        tr("QtEmu &Quick Help"),
                                        this);
    m_helpQuickHelpAction->setShortcut(Qt::Key_F1);
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
    m_startMachineAction->setIcon(QIcon::fromTheme("media-playback-start",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/start.svg"))));
    m_startMachineAction->setToolTip(tr("Start machine"));
    connect(m_startMachineAction, &QAction::triggered,
            this, &MainWindow::runMachine);

    m_stopMachineAction = new QAction(this);
    m_stopMachineAction->setIcon(QIcon::fromTheme("media-playback-stop",
                                                  QIcon(QPixmap(":/images/icons/breeze/32x32/stop.svg"))));
    m_stopMachineAction->setToolTip(tr("Stop machine"));

    m_resetMachineAction = new QAction(this);
    m_resetMachineAction->setIcon(QIcon::fromTheme("chronometer-reset",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/chronometer-reset.svg"))));
    m_resetMachineAction->setToolTip(tr("Reset machine"));
    connect(m_resetMachineAction, &QAction::triggered,
            this, &MainWindow::resetMachine);

    m_pauseMachineAction = new QAction(this);
    m_pauseMachineAction->setIcon(QIcon::fromTheme("media-playback-pause",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/pause.svg"))));
    m_pauseMachineAction->setToolTip(tr("Pause machine"));
    connect(m_pauseMachineAction, &QAction::triggered,
            this, &MainWindow::pauseMachine);
}

/**
 * @brief Create the toolbar
 *
 * Create the main toolbar of the app
 */
void MainWindow::createToolBars()
{
    this->m_mainToolBar = addToolBar(tr("Toolbar"));

    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    m_mainToolBar->setMovable(false);
    m_mainToolBar->addAction(this->m_newMachineAction);
    m_mainToolBar->addAction(this->m_settingsMachineAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(this->m_startMachineAction);
    m_mainToolBar->addAction(this->m_stopMachineAction);
    m_mainToolBar->addAction(this->m_resetMachineAction);
    m_mainToolBar->addAction(this->m_pauseMachineAction);
    m_mainToolBar->addAction(this->m_killMachineAction);

}

/**
 * @brief Open QtEmu Website
 *
 * Open the QtEmu official webpage into your browser
 */
void MainWindow::visitQtEmuWebsite()
{
    QDesktopServices::openUrl(QUrl("https://www.qtemu.org"));
}

/**
 * @brief Open QtEmu Bug Tracker
 *
 * Open the QtEmu Bug Tracker into your browser
 */
void MainWindow::visitQtEmuBugTracker()
{
    QDesktopServices::openUrl(QUrl("https://gitlab.com/qtemu/gui/-/issues"));
}

/**
 * @brief Open Qemu Website
 *
 * Open the Qemu official webpage into your browser
 */
void MainWindow::visitQemuWebsite()
{
    QDesktopServices::openUrl(QUrl("https://www.qemu.org/"));
}

/**
 * @brief Check QEMU and QtEmu version
 *
 * Check QEMU and QtEmu version
 */
void MainWindow::checkVersions()
{
    QNetworkRequest request(QUrl("https://www.carlavilla.es/docs/qtemu.json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager networkAccessManager;
    QNetworkReply *reply = networkAccessManager.get(request);

    while(!reply->isFinished()) {
        qApp->processEvents();
    }

    if (reply->error()) {        
        SystemUtils::showMessage(tr("QtEmu - Network problem"),
                                 tr("<p>There's a network problem</p>"),
                                 QMessageBox::Critical);
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument qemuQtEmuVersion = QJsonDocument::fromJson(response);
    QString qemuVersion = qemuQtEmuVersion["qemu"].toString();
    QString qtemuVersion = qemuQtEmuVersion["qtemu"].toString();
    QString installedQtEmuVersion = QCoreApplication::applicationVersion();

    reply->deleteLater();

    SystemUtils::showMessage(tr("QtEmu - versions"),
                             tr("<p><strong>QtEmu installed version: </strong>") + installedQtEmuVersion + "</p>" +
                             tr("<p><strong>Last QtEmu version: </strong>") + qtemuVersion + "</p>",
                             QMessageBox::Critical);
}

/**
 * @brief Close the app
 *
 * Close the app
 */
void MainWindow::quitApp()
{
    if (this->isHidden()) {
        this->show();
    }

    QMessageBox::StandardButton confirmation = QMessageBox::question(this,
                               tr("Quit?"),
                               tr("Do you really want to close QtEmu?\nIf there are machines running there are going to close"),
                               QMessageBox::Yes | QMessageBox::No,
                               QMessageBox::No);

     if (confirmation == QMessageBox::Yes) {
        qApp->setQuitOnLastWindowClosed(true);
        qApp->closeAllWindows();
        qApp->quit();
    }
}

/**
 * @brief Load created machines
 *
 * Load all the machines stored in the qtemu.json file on config data folder
 */
void MainWindow::loadMachines()
{
    QSettings settings;
    settings.beginGroup("DataFolder");
    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");
    QFile machinesFile(qtemuConfig);
    // If file don't exists I asume that there's no machines
    if (!machinesFile.exists()) {
        return;
    }

    if (!machinesFile.open(QIODevice::ReadOnly)) {        
        SystemUtils::showMessage(tr("QtEmu - Critical error"),
                                 tr("<p><strong>Cannot load the saved machines</strong></p>"
                                    "<p>Cannot open the <strong>qtemu.json</strong> file. "
                                    "The saved machines cannot be loaded. "
                                    "Please ensure that the file exists and it's readable</p>"),
                                 QMessageBox::Critical);
        return;
    }

    // Read all data included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonArray machines = machinesDocument["machines"].toArray();
    for (int i = 0; i < machines.size(); ++i) {
        this->generateMachineObject(machines[i].toObject(), i);
    }

    if (machinesFile.isOpen()) {
        machinesFile.close();
    }
}

/**
 * @brief Generate the machine object for the list
 * @param machineConfigJsonObject, JSON with the machine configpath, icon, path and uuid
 * @param pos, pos of the machine in the list
 * @return machine object
 *
 * Generate the machine object for the list
 */
void MainWindow::generateMachineObject(const QJsonObject machineConfigJsonObject, int pos)
{
    QString machineConfigPath = machineConfigJsonObject["configpath"].toString();
    QJsonObject machineJSON = MachineUtils::readMachineFile(machineConfigPath);

    if (machineJSON.isEmpty()) {
        return;
    }

    QListWidgetItem *machineListItem = new QListWidgetItem(machineJSON["name"].toString(), this->m_osListWidget);
    machineListItem->setData(QMetaType::QUuid, machineJSON["uuid"].toString());
    machineListItem->setIcon(QIcon(":/images/os/64x64/" +
                                   SystemUtils::getOsIcon(machineConfigJsonObject["icon"].toString())));

    // To prevent undefined behavior :'(
    if (pos == 0) {
        this->m_osListWidget->setCurrentItem(machineListItem);
    }

    Machine *machine = new Machine(this);
    connect(machine, &Machine::machineStateChangedSignal,
            this, &MainWindow::machineStateChanged);

    MachineUtils::fillMachineObject(machine,
                                    machineJSON,
                                    machineConfigPath);

    this->m_machinesList.append(machine);
}

/**
 * @brief Open the create machine wizard
 *
 * Open the machine wizard to create a new machine
 */
void MainWindow::createNewMachine()
{
    m_machine = new Machine(this);
    m_machine->addAudio("ac97");
    m_machine->setRAM(0);
    m_machine->setSocketCount(0);
    m_machine->setCoresSocket(0);
    m_machine->setThreadsCore(0);
    m_machine->setMaxHotCPU(0);
    m_machine->setState(Machine::Stopped);

    connect(m_machine, &Machine::machineStateChangedSignal,
            this, &MainWindow::machineStateChanged);

    MachineWizard newMachineWizard(m_machine, this->m_osListWidget, this->qemuGlobalObject, this);

    newMachineWizard.show();
    newMachineWizard.exec();

    if (!m_machine->getUuid().isNull()) {
        m_machinesList.append(m_machine);
        this->loadUI(this->m_osListWidget->count());
    }
}

/**
 * @brief Delete the selected machine
 *
 * Delete the selected machine and its associated files
 */
void MainWindow::deleteMachine()
{
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    bool isMachineDeleted = MachineUtils::deleteMachine(machineUuid);
    if (isMachineDeleted) {
        this->m_osListWidget->takeItem(this->m_osListWidget->currentRow());
        bool machineRemovedList = false;
        QMutableListIterator<Machine*> machines(this->m_machinesList);
        while (machines.hasNext() && !machineRemovedList) {
            if (machines.next()->getUuid() == machineUuid) {
                machines.remove();
                machineRemovedList = true;
            }
        }
        this->m_osListWidget->setCurrentRow(0);
        this->loadUI(this->m_osListWidget->count());
    }
}

/**
 * @brief Open the machine options window
 *
 * Open the selected machine options window
 */
void MainWindow::machineOptions()
{
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    Machine *machineOptions = nullptr;
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            machineOptions = machine;
            break;
        }
    }

    m_machineConfigWindow = new MachineConfigWindow(machineOptions,
                                                    this->qemuGlobalObject,
                                                    this->m_osListWidget->currentItem(),
                                                    this);
    m_machineConfigWindow->show();

    connect(m_machineConfigWindow, &MachineConfigWindow::saveMachineSettingsSignal,
            this, &MainWindow::updateMachineDetailsConfig);
}

/**
 * @brief Export the selected machine
 *
 * Export the selected machine
 */
void MainWindow::exportMachine()
{
    // TODO: Move that code to a function...
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    QString machineConfigPath;
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            machineConfigPath = machine->getConfigPath();
            break;
        }
    }

    if (!machineConfigPath.isEmpty()) {
        QJsonObject machineJSON = MachineUtils::readMachineFile(machineConfigPath);
        Machine *exportMachine = new Machine(this);
        MachineUtils::fillMachineObject(exportMachine, machineJSON, machineConfigPath);

        ExportWizard exportWizard(exportMachine, this);
        exportWizard.show();
        exportWizard.exec();

        delete exportMachine;
    }
}

/**
 * @brief Import machine wizard
 *
 * Import machine wizard
 */
void MainWindow::importMachine()
{
    Machine *machine = new Machine(this);
    connect(machine, &Machine::machineStateChangedSignal,
            this, &MainWindow::machineStateChanged);

    ImportWizard importWizard(machine, this->m_osListWidget, this);

    importWizard.show();
    importWizard.exec();

    if (machine->getConfigPath().isEmpty()) {
        delete machine;
        return;
    } else {
        this->m_machinesList.append(machine);
        this->loadUI(this->m_osListWidget->count());
    }
}

/**
 * @brief Start the selected machine
 *
 * Start the selected machine
 */
void MainWindow::runMachine()
{
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            machine->runMachine(this->qemuGlobalObject);
            break;
        }
    }
}

/**
 * @brief Reset the selected machine
 *
 * Reset the selected machine
 */
void MainWindow::resetMachine()
{
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            machine->resetMachine();
            break;
        }
    }
}

/**
 * @brief Pause or continue the selected machine
 *
 * If the State of the machine is Started, then
 * pause the machine.
 * If the State of the machine is Paused, then
 * continue the execution of the machine
 */
void MainWindow::pauseMachine()
{
    QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            machine->pauseMachine();
            break;
        }
    }
}

/**
 * @brief Enable/Disable buttons
 *
 * Enable/Disable the buttons in the menubar and in the main ui
 * If the osListWidget item doesn't have elements, elements
 * related to the VM actions are disabled. If the osListWidget have
 * at least one element, elements are enabled
 *
 * @param machineCount, number of machines
 */
void MainWindow::loadUI(const int machineCount)
{
    if (machineCount == 0) {
        // Disable all options
        this->m_startMachineAction->setEnabled(false);
        this->m_stopMachineAction->setEnabled(false);
        this->m_resetMachineAction->setEnabled(false);
        this->m_pauseMachineAction->setEnabled(false);
        this->m_settingsMachineAction->setEnabled(false);
        this->m_exportMachineAction->setEnabled(false);
        this->m_removeMachineAction->setEnabled(false);

        this->emptyMachineDetailsSection();
    } else {
        QUuid machineUuid = this->m_osListWidget->currentItem()->data(QMetaType::QUuid).toUuid();
        for (Machine *machine : this->m_machinesList) {
            if (machine->getUuid() == machineUuid){
                this->m_settingsMachineAction->setEnabled(true);
                this->m_exportMachineAction->setEnabled(true);
                this->m_removeMachineAction->setEnabled(true);
                this->controlMachineActions(machine->getState());
                this->fillMachineDetailsSection(machine);
                break;
            }
        }
    }
}

/**
 * @brief Enable or disable the machine action items
 * @param machineItem, machine item of the list
 *
 * Enable/Disable the machine action items depending the
 * state of the machine
 */
void MainWindow::changeMachine(QListWidgetItem *machineItem)
{
    QUuid machineUuid = machineItem->data(QMetaType::QUuid).toUuid();
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid) {
            controlMachineActions(machine->getState());
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
void MainWindow::fillMachineDetailsSection(Machine *machine)
{
    this->m_machineNameLabel->setText(machine->getName());
    this->m_machineOsLabel->setText(machine->getOSType() + " - " + machine->getOSVersion());
    this->m_machineCPULabel->setText(machine->getCPUType());
    this->m_machineRAMLabel->setText(QString::number(machine->getRAM()) + " MiB");
    this->m_machineGraphicsLabel->setText(machine->getGPUType());
    this->m_machineAudioLabel->setText(machine->getAudioLabel());
    this->m_machineAccelLabel->setText(machine->getAcceleratorLabel());
    this->m_machineNetworkLabel->setText(machine->getUseNetwork() == true ? tr("Yes") : tr("no"));
    QString mediaLabel;
    for (int i = 0; i < machine->getMedia().size(); ++i) {
         mediaLabel.append("(")
                   .append(machine->getMedia().at(i)->driveInterface().toUpper())
                   .append(") ")
                   .append(machine->getMedia().at(i)->name())
                   .append("\n");
    }
    this->m_machineMediaLabel->setText(mediaLabel);
}

/**
 * @brief Empty the machine details section of the main UI
 *
 * Empty the machine details section of the main UI
 * when there's no machines
 */
void MainWindow::emptyMachineDetailsSection()
{
    this->m_machineNameLabel->setText("");
    this->m_machineOsLabel->setText("");
    this->m_machineCPULabel->setText("");
    this->m_machineRAMLabel->setText("");
    this->m_machineGraphicsLabel->setText("");
    this->m_machineAudioLabel->setText("");
    this->m_machineAccelLabel->setText("");
    this->m_machineNetworkLabel->setText("");
    this->m_machineMediaLabel->setText("");
}

/**
 * @brief Show a machine's menu
 * @param pos, position
 *
 * Show a machine's menu when the user press
 * the second button of the mouse or touchpad
 */
void MainWindow::machinesMenu(const QPoint &pos)
{
    this->m_machineMenu->exec(this->m_osListWidget->mapToGlobal(pos));
}

/**
 * @brief Control when the state of a VM changes
 * @param newState, new state of the VM
 *
 * Control when the state of a VM changes
 */
void MainWindow::machineStateChanged(Machine::States newState)
{
    controlMachineActions(newState);
}

/**
 * @brief Control if the state of the actions of the VM
 * @param state, state of the VM
 *
 * Control if the actions of the machines are enabled or disabled
 * based on the state of the VM
 */
void MainWindow::controlMachineActions(Machine::States state)
{
    if (state == Machine::Started) {
        this->m_startMachineAction->setEnabled(false);
        this->m_stopMachineAction->setEnabled(true);
        this->m_resetMachineAction->setEnabled(true);
        this->m_pauseMachineAction->setEnabled(true);
        this->m_killMachineAction->setEnabled(true);
    } else if (state == Machine::Stopped) {
        this->m_startMachineAction->setEnabled(true);
        this->m_stopMachineAction->setEnabled(false);
        this->m_resetMachineAction->setEnabled(false);
        this->m_pauseMachineAction->setEnabled(false);
    } else if(state == Machine::Paused) {
        this->m_startMachineAction->setEnabled(false);
        this->m_stopMachineAction->setEnabled(false);
        this->m_resetMachineAction->setEnabled(false);
        this->m_pauseMachineAction->setEnabled(true);
    }
}

/**
 * @brief Update the machine details
 * @param machineUuid, uuid of the selected machine
 *
 * Update the machine details after finish the
 * configuration of a machine
 */
void MainWindow::updateMachineDetailsConfig(const QUuid machineUuid)
{
    for (Machine *machine : this->m_machinesList) {
        if (machine->getUuid() == machineUuid){
            this->fillMachineDetailsSection(machine);
            break;
        }
    }
}

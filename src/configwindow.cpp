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
#include "configwindow.h"

/**
 * @brief QtEmu configuration menu
 * @param parent, widget parent
 *
 * Configuration of QtEmu. Configure the machines path, language,
 * qemu before and after commands and the proxy.
 */
ConfigWindow::ConfigWindow(QEMU *QEMUGlobalObject, QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(tr("Preferences") + " - QtEmu");
    this->setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/images/qtemu.png")));
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::ApplicationModal);
    this->setMinimumSize(640, 520);

    this->m_QEMUObject = QEMUGlobalObject;

    this->createGeneralPage();
    this->createUpdatePage();
    this->createLanguagePage();
    //this->createStartPage(); QtEmu 2.x
    this->createProxyPage();
    this->createQEMUPage();

    m_optionsListWidget = new QListWidget(this);
    m_optionsListWidget->setViewMode(QListView::ListMode);
    m_optionsListWidget->setIconSize(QSize(32, 32));
    m_optionsListWidget->setMovement(QListView::Static);
    m_optionsListWidget->setMaximumWidth(170);
    m_optionsListWidget->setSpacing(7);
    m_optionsListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add options
    m_optionsListWidget->addItem(tr("General Options"));
    m_optionsListWidget->item(0)->setIcon(QIcon::fromTheme("preferences-other",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));

    m_optionsListWidget->addItem(tr("Update QtEmu"));
    m_optionsListWidget->item(1)->setIcon(QIcon::fromTheme("update-none",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/update-none.svg"))));

    m_optionsListWidget->addItem(tr("Language"));
    m_optionsListWidget->item(2)->setIcon(QIcon::fromTheme("applications-education-language",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/applications-education-language.svg"))));

    // QtEmu 2.x
    //m_optionsListWidget->addItem(tr("Start"));
    //m_optionsListWidget->item(3)->setIcon(QIcon::fromTheme("practice-start",
    //                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/practice-start.svg"))));

    m_optionsListWidget->addItem(tr("Proxy"));
    m_optionsListWidget->item(3)->setIcon(QIcon::fromTheme("network-manager",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    m_optionsListWidget->addItem(tr("QEMU"));
    m_optionsListWidget->item(4)->setIcon(QIcon(QPixmap(":/images/QEMU.png")));

    // Prepare window
    m_categoriesStackedWidget = new QStackedWidget(this);
    m_categoriesStackedWidget->setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::MinimumExpanding);

    m_categoriesStackedWidget->addWidget(this->m_generalPageWidget);
    m_categoriesStackedWidget->addWidget(this->m_updatePageWidget);
    m_categoriesStackedWidget->addWidget(this->m_languagePageWidget);
    //m_categoriesStackedWidget->addWidget(this->m_startPageWidget); QtEmu 2.x
    m_categoriesStackedWidget->addWidget(this->m_proxyPageWidget);
    m_categoriesStackedWidget->addWidget(this->m_QEMUPageWidget);

    connect(m_optionsListWidget, &QListWidget::currentRowChanged,
            m_categoriesStackedWidget, &QStackedWidget::setCurrentIndex);

    m_topLayout = new QHBoxLayout();
    m_topLayout->addWidget(m_optionsListWidget);
    m_topLayout->addWidget(m_categoriesStackedWidget);

    // Buttons
    m_saveButton = new QPushButton(QIcon::fromTheme("document-save",
                                                    QIcon(QPixmap(":/images/icons/breeze/32x32/document-save.svg"))),
                                   tr("Save"),
                                   this);
    connect(m_saveButton, &QAbstractButton::clicked,
            this, &ConfigWindow::saveSettings);

    m_closeButton = new QPushButton(QIcon::fromTheme("dialog-cancel",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))),
                                    tr("Cancel"),
                                    this);
    connect(m_closeButton, &QAbstractButton::clicked,
            this, &ConfigWindow::cancelButton);

    this->m_buttonsLayout = new QHBoxLayout();
    m_buttonsLayout->setAlignment(Qt::AlignRight);
    m_buttonsLayout->addWidget(m_saveButton);
    m_buttonsLayout->addWidget(m_closeButton);

    m_closeAction = new QAction(this);
    m_closeAction->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(m_closeAction, &QAction::triggered, this, &ConfigWindow::cancelButton);
    this->addAction(m_closeAction);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addLayout(m_topLayout, 20);
    m_mainLayout->addSpacing(8);
    m_mainLayout->addStretch(1);
    m_mainLayout->addLayout(m_buttonsLayout);

    this->setLayout(m_mainLayout);

    this->m_optionsListWidget->setCurrentRow(0);
    this->m_optionsListWidget->setFocus();

    // Load settings
    loadSettings();

    qDebug() << "ConfigWindow created";
}

ConfigWindow::~ConfigWindow()
{
    qDebug() << "ConfigWindow destroyed";
}

/**
 * @brief Event triggered when the user close the window
 * @param event, close event
 *
 * Event triggered when the user close the window.
 * Load the settings saved before by the user.
 */
void ConfigWindow::closeEvent (QCloseEvent *event)
{
    this->loadSettings();
    this->hide();
    this->m_optionsListWidget->setCurrentRow(0);
    event->accept();
}

/**
 * @brief Create the general page of the QtEmu configuration
 *
 * Create the general page of the QtEmu configuration where
 * the machines path are indicated.
 */
void ConfigWindow::createGeneralPage()
{
    m_machinePathLabel  = new QLabel(tr("Default machine path") + ":", this);
    m_machinePathLineEdit = new QLineEdit(this);
    m_machinePathLineEdit->setEnabled(false);

    m_machinePathButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                          "",
                                          this);

    connect(m_machinePathButton, &QAbstractButton::clicked,
            this, &ConfigWindow::setMachinePath);

#ifdef Q_OS_WIN
    m_monitorHostnameLabel = new QLabel(tr("Hostname") + ":", this);
    m_monitorHostnameLabel->setWordWrap(true);

    m_monitorHostnameComboBox = new QComboBox(this);
    m_monitorHostnameComboBox->addItem("localhost");
    m_monitorHostnameComboBox->addItem("127.0.0.1");

    m_monitorSocketPathLabel = new QLabel(tr("Port") + ":", this);
    m_monitorSocketSpinBox = new QSpinBox(this);
    m_monitorSocketSpinBox->setMinimum(1);
    m_monitorSocketSpinBox->setMaximum(65535);
    m_monitorSocketSpinBox->setValue(6000);
#endif

    m_machinePathLayout = new QHBoxLayout();
    m_machinePathLayout->setAlignment(Qt::AlignTop);
    m_machinePathLayout->addWidget(m_machinePathLabel);
    m_machinePathLayout->addWidget(m_machinePathLineEdit);
    m_machinePathLayout->addWidget(m_machinePathButton);

#ifdef Q_OS_WIN
    m_machineSocketLayout = new QHBoxLayout();
    m_machineSocketLayout->addWidget(m_monitorHostnameLabel);
    m_machineSocketLayout->addWidget(m_monitorHostnameComboBox);

    m_machinePortSocketLayout = new QHBoxLayout();
    m_machinePortSocketLayout->addWidget(m_monitorSocketPathLabel);
    m_machinePortSocketLayout->addWidget(m_monitorSocketSpinBox);
#endif

    m_groupLayout = new QVBoxLayout();
    m_groupLayout->setAlignment(Qt::AlignTop);
    m_groupLayout->addItem(m_machinePathLayout);

    m_machinePathGroup = new QGroupBox(tr("Machine Path"), this);
    m_machinePathGroup->setLayout(m_groupLayout);
    m_machinePathGroup->setFlat(false);

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout->setAlignment(Qt::AlignTop);
    m_generalPageLayout->addWidget(m_machinePathGroup);
#ifdef Q_OS_WIN
    m_generalPageLayout->addItem(m_machineSocketLayout);
    m_generalPageLayout->addItem(m_machinePortSocketLayout);
#endif

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget->setLayout(m_generalPageLayout);
}

/**
 * @brief Create the update page of the QtEmu configuration
 *
 * Create the update page of the QtEmu configuration where
 * the version of the software can be selected.
 * Versions: Stable, Beta, Development
 */
void ConfigWindow::createUpdatePage()
{
    m_updateCheckBox = new QCheckBox(this);
    m_updateCheckBox->setChecked(true);

    connect(m_updateCheckBox, &QAbstractButton::toggled,
                this, &ConfigWindow::toggleUpdate);

    m_updatesGroup = new QGroupBox(this);

    m_stableReleaseRadio = new QRadioButton(tr("Stable version"), this);
    m_betaReleaseRadio = new QRadioButton(tr("Beta version"), this);
    m_developmentRelaseRadio = new QRadioButton(tr("Development version"), this);

    m_stableReleaseRadio->setChecked(true);

    connect(m_stableReleaseRadio, &QAbstractButton::toggled,
                this, &ConfigWindow::pushStableVersion);

    m_updateRadiosLayout = new QVBoxLayout();
    m_updateRadiosLayout->setAlignment(Qt::AlignVCenter);
    m_updateRadiosLayout->addWidget(m_stableReleaseRadio);
    m_updateRadiosLayout->addWidget(m_betaReleaseRadio);
    m_updateRadiosLayout->addWidget(m_developmentRelaseRadio);

    m_updatesGroup->setLayout(m_updateRadiosLayout);

    m_updatePageLayout = new QFormLayout();
    m_updatePageLayout->addRow(tr("Check for updates"), m_updateCheckBox);
    m_updatePageLayout->addRow(m_updatesGroup);

    m_updatePageWidget = new QWidget(this);
    m_updatePageWidget->setLayout(m_updatePageLayout);
}

/**
 * @brief Create the language page of the QtEmu configuration
 *
 * Create the language page of the QtEmu configuration where
 * the language can be selected.
 */
void ConfigWindow::createLanguagePage()
{
    m_languageLabel = new QLabel(tr("Interface language"), this);

    m_languagesListView = new QListWidget(this);
    m_languagesListView->setViewMode(QListView::ListMode);
    m_languagesListView->setIconSize(QSize(32, 32));
    m_languagesListView->setMovement(QListView::Static);
    m_languagesListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_languagesListView->addItem("English");
    m_languagesListView->item(0)->setIcon(QIcon(":/images/flags/32x32/eeuu.png"));

    m_languageDescription = new QLabel(this);
    m_languageAuthors = new QLabel(this);

    connect(m_languagesListView, &QListWidget::currentTextChanged,
            this, &ConfigWindow::setLanguageLabel);

    connect(m_languagesListView, &QListWidget::currentRowChanged,
            this, &ConfigWindow::setAuthorsLabel);

    m_languagePageLayout = new QVBoxLayout();
    m_languagePageLayout->addWidget(m_languageLabel);
    m_languagePageLayout->addWidget(m_languagesListView);

    m_languageHLayout = new QHBoxLayout();
    m_languageHLayout->addWidget(m_languageDescription);
    m_languageHLayout->addWidget(m_languageAuthors);

    m_languagePageLayout->addItem(m_languageHLayout);

    m_languagePageWidget = new QWidget(this);
    m_languagePageWidget->setLayout(m_languagePageLayout);
}

/**
 * @brief Create the start page of the QtEmu configuration
 *
 * Create the start page of the QtEmu configuration where
 * can be configured wich commands execute before and after
 * the launch of QEMU.
 */
// QtEmu 2.x
/*void ConfigWindow::createStartPage()
{
    m_beforeStartLabel = new QLabel(tr("Execute before start") + ":", this);
    m_afterExitLabel = new QLabel(tr("Execute after exit") + ":", this);

    m_beforeStart = new QPlainTextEdit(this);
    m_afterExit = new QPlainTextEdit(this);

    m_startPageLayout = new QVBoxLayout();

    m_startPageLayout->addWidget(m_beforeStartLabel);
    m_startPageLayout->addWidget(m_beforeStart);
    m_startPageLayout->addWidget(m_afterExitLabel);
    m_startPageLayout->addWidget(m_afterExit);

    m_startPageWidget = new QWidget(this);
    m_startPageWidget->setLayout(m_startPageLayout);
}*/

/**
 * @brief Create the proxy page of the QtEmu configuration
 *
 * Create the proxy page of the QtEmu configuration where the
 * proxy options can be configured.
 */
void ConfigWindow::createProxyPage()
{
    m_proxyOptions = new QComboBox(this);
    m_proxyOptions->addItem(tr("Do not use a proxy"));
    m_proxyOptions->addItem("SOCKS 5");
    m_proxyOptions->addItem("HTTP");
    m_proxyOptions->setCurrentIndex(0);

    m_serverNameProxy = new QLineEdit(this);
    m_serverNameProxy->setPlaceholderText("example.org");

    m_portProxy = new QLineEdit(this);
    m_portProxy->setPlaceholderText("1080, 8080, etc...");

    connect(m_proxyOptions, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ConfigWindow::toggleServerPort);

    m_useAuth = new QCheckBox(this);
    m_useAuth->setChecked(false);

    connect(m_useAuth, &QAbstractButton::toggled,
                this, &ConfigWindow::toggleAuth);

    m_userProxy = new QLineEdit(this);
    m_userProxy->setPlaceholderText(tr("Proxy username"));

    m_passwordProxy = new QLineEdit(this);
    m_passwordProxy->setEchoMode(QLineEdit::Password);

    m_proxyPageLayout = new QFormLayout();
    m_proxyPageLayout->addRow(tr("Proxy type"), m_proxyOptions);
    m_proxyPageLayout->addRow(tr("Hostname"), m_serverNameProxy);
    m_proxyPageLayout->addRow(tr("Port"), m_portProxy);
    m_proxyPageLayout->addRow(tr("Use Authentication"), m_useAuth);
    m_proxyPageLayout->addRow(tr("User"), m_userProxy);
    m_proxyPageLayout->addRow(tr("Password"), m_passwordProxy);

    m_proxyPageWidget = new QWidget(this);
    m_proxyPageWidget->setLayout(m_proxyPageLayout);
}

/**
 * @brief Create the QEMU page of the QtEmu configuration
 *
 * Create the QEMU page of the QtEmu configuration where the
 * QEMU related options can be configured. Binaries, Version of QEMU, qemu-img path, etc.
 */
void ConfigWindow::createQEMUPage()
{
    m_findBinaryLabel = new QLabel(tr("QEMU binaries path"), this);
    m_binaryPathLineEdit = new QLineEdit(this);
    connect(m_binaryPathLineEdit, &QLineEdit::textChanged,
            this, &ConfigWindow::binaryPathChanged);

    m_binariesPathToolButton = new QToolButton(this);
    m_binariesPathToolButton->setToolTip(tr("QEMU binaries path"));
    m_binariesPathToolButton->setToolTipDuration(3000);
    m_binariesPathToolButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/window-close.svg"))));
    connect(m_binariesPathToolButton, &QAbstractButton::clicked,
            this, &ConfigWindow::setPathBinaries);

    m_searchBinariesToolButton = new QToolButton(this);
    m_searchBinariesToolButton->setEnabled(false);
    m_searchBinariesToolButton->setToolTip(tr("Find binaries"));
    m_searchBinariesToolButton->setToolTipDuration(3000);
    m_searchBinariesToolButton->setIcon(QIcon::fromTheme("edit-find",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/window-close.svg"))));
    connect(m_searchBinariesToolButton, &QAbstractButton::clicked,
            this, &ConfigWindow::findBinaries);

    QStringList labels;
    labels << "Name" << "Path";

    m_binariesTableWidget = new QTableWidget(this);
    m_binariesTableWidget->setColumnCount(2);
    m_binariesTableWidget->setColumnWidth(0, 150);
    m_binariesTableWidget->setColumnWidth(1, 220);
    m_binariesTableWidget->setLayoutDirection(Qt::LeftToRight);
    m_binariesTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_binariesTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_binariesTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_binariesTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_binariesTableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_binariesTableWidget->setHorizontalHeaderLabels(labels);

    m_QEMUImgLabel = new QLabel(tr("QEMU img binary path"), this);
    m_QEMUImgPathLineEdit = new QLineEdit(this);

    m_searchQEMUImgToolButton = new QToolButton(this);
    m_searchQEMUImgToolButton->setToolTip(tr("Find qemu-img binary"));
    m_searchQEMUImgToolButton->setToolTipDuration(3000);
    m_searchQEMUImgToolButton->setIcon(QIcon::fromTheme("edit-find",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/window-close.svg"))));
    connect(m_searchQEMUImgToolButton, &QAbstractButton::clicked,
            this, &ConfigWindow::findQemuImgBinary);

    m_binaryLabelLayout = new QHBoxLayout();
    m_binaryLabelLayout->setAlignment(Qt::AlignLeft);
    m_binaryLabelLayout->addWidget(m_findBinaryLabel);

    m_binaryLayout = new QHBoxLayout();
    m_binaryLayout->setAlignment(Qt::AlignCenter);
    m_binaryLayout->addWidget(m_binaryPathLineEdit);
    m_binaryLayout->addWidget(m_binariesPathToolButton);
    m_binaryLayout->addWidget(m_searchBinariesToolButton);

    m_QEMUPathLayout = new QHBoxLayout();
    m_QEMUPathLayout->setAlignment(Qt::AlignCenter);
    m_QEMUPathLayout->addWidget(m_QEMUImgPathLineEdit);
    m_QEMUPathLayout->addWidget(m_searchQEMUImgToolButton);

    m_QEMUImgLayout = new QVBoxLayout();
    m_QEMUImgLayout->setAlignment(Qt::AlignLeft);
    m_QEMUImgLayout->addWidget(m_QEMUImgLabel);
    m_QEMUImgLayout->addItem(m_QEMUPathLayout);

    m_QEMULayout = new QVBoxLayout();
    m_QEMULayout->addItem(m_binaryLabelLayout);
    m_QEMULayout->addItem(m_binaryLayout);
    m_QEMULayout->addWidget(m_binariesTableWidget);
    m_QEMULayout->addItem(m_QEMUImgLayout);

    m_QEMUPageWidget = new QWidget(this);
    m_QEMUPageWidget->setLayout(m_QEMULayout);
}

/**
 * @brief Insert all the QEMU binaries
 *
 * Insert all the QEMU binaries into the QTableWidget
 */
void ConfigWindow::insertBinariesInTree()
{
    this->m_binariesTableWidget->clearContents();
    this->m_binariesTableWidget->setRowCount(0);

    QMapIterator<QString, QString> iterator(this->m_QEMUObject->QEMUBinaries());
    while (iterator.hasNext()) {
        iterator.next();
        this->m_binariesTableWidget->insertRow(this->m_binariesTableWidget->rowCount());

        QTableWidgetItem *binaryItem = new QTableWidgetItem(iterator.key());
        this->m_binariesTableWidget->setItem(this->m_binariesTableWidget->rowCount()-1, 0, binaryItem);

        binaryItem = new QTableWidgetItem(iterator.value());
        this->m_binariesTableWidget->setItem(this->m_binariesTableWidget->rowCount()-1, 1, binaryItem);
    }
}

/**
 * @brief Enable or disabled the update radios
 * @param updateState
 *
 * Indicate if the radios referred to the update section are enabled or disabled
 */
void ConfigWindow::toggleUpdate(bool updateState)
{
    this->m_updatesGroup->setEnabled(updateState);
    this->m_stableReleaseRadio->setEnabled(updateState);
    this->m_betaReleaseRadio->setEnabled(updateState);
    this->m_developmentRelaseRadio->setEnabled(updateState);
}

/**
 * @brief Select stable version
 * @param release
 *
 * Select stable version for the QtEmu release chanel
 */
void ConfigWindow::pushStableVersion(bool release)
{
    if (release) {
        this->m_releaseString = "stable";
    }
}

/**
 * @brief Select beta version
 * @param release
 *
 * Select beta version for the QtEmu release chanel
 */
void ConfigWindow::pushBetaVersion(bool release)
{
    if (release) {
        this->m_releaseString = "beta";
    }
}

/**
 * @brief Select development version
 * @param release
 *
 * Select development version for the QtEmu release chanel
 */
void ConfigWindow::pushDevelopmentVersion(bool release)
{
    if (release) {
        this->m_releaseString = "alpha";
    }
}

/**
 * @brief Set the language label
 * @param language, language string
 *
 * Set the language label
 */
void ConfigWindow::setLanguageLabel(QString language)
{
    QString description = tr("Language") + ": ";
    description.append(language);

    this->m_languageDescription->setText(description);
}

/**
 * @brief Set the name of the translator
 * @param languagePosition, position of the language
 *
 * Set the name of the translator
 */
void ConfigWindow::setAuthorsLabel(int languagePosition)
{
    QString authors = tr("Authors") + ": ";

    switch (languagePosition) {
        case 0:
            authors.append(tr("QtEmu Developers"));
            this->m_languageISOCode = "en";
            this->m_languagePos = 0;
            break;
        default:
            authors.append(tr("Unknown author"));
            this->m_languageISOCode = "en";
            this->m_languagePos = 0;
            break;
    }

    this->m_languageAuthors->setText(authors);
}

/**
 * @brief Activate or deactivate the server and port
 * @param proxyOption
 *
 * Activate or deactivate the server and port for
 * QtEmu
 */
void ConfigWindow::toggleServerPort(int proxyOption)
{
    if (proxyOption == 0) {
        this->m_serverNameProxy->setEnabled(false);
        this->m_portProxy->setEnabled(false);
    } else {
        this->m_serverNameProxy->setEnabled(true);
        this->m_portProxy->setEnabled(true);
    }
}

/**
 * @brief Establish user and password for proxy settings
 * @param authState, activate or deactivate the user and password inputs
 *
 * Establish user and password for proxy settings
 */
void ConfigWindow::toggleAuth(bool authState)
{

    if (!authState) {
        this->m_userProxy->setText("");
        this->m_passwordProxy->setText("");
    }

    this->m_userProxy->setEnabled(authState);
    this->m_passwordProxy->setEnabled(authState);
}

/**
 * @brief Set the path for the machines
 *
 * Establish the path where the machines are
 * going to be located
 */
void ConfigWindow::setMachinePath()
{
    this->m_machinePath = QFileDialog::getExistingDirectory(this, tr("Select a folder for Machines"),
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks
                                                            );
    if (!m_machinePath.isEmpty()) {
        this->m_machinePathLineEdit->setText(QDir::toNativeSeparators(m_machinePath));
    }
}

/**
 * @brief Action triggered when button cancel is pressed
 *
 * Action trigerred when the button cancel is pressed.
 * Load the settings saved before by the user.
 */
void ConfigWindow::cancelButton()
{
    this->loadSettings();
    this->m_searchBinariesToolButton->setEnabled(false);

    QSettings settings;
    settings.beginGroup("Configuration");
    this->m_QEMUObject->setQEMUBinaries(settings.value("qemuBinaryPath", "").toString());
    settings.endGroup();

    this->insertBinariesInTree();
    this->hide();
    this->m_optionsListWidget->setCurrentRow(0);

    qDebug() << "Config window canceled";
}

/**
 * @brief Save the QtEmu settings
 *
 * Load the QtEmu settings
 */
void ConfigWindow::saveSettings()
{
    QSettings settings;

    if (!settings.isWritable()) {
        return;
    }

    settings.beginGroup("Configuration");

    // General
    settings.setValue("machinePath", this->m_machinePathLineEdit->text());
#ifdef Q_OS_WIN
    settings.setValue("qemuMonitorHost", this->m_monitorHostnameComboBox->currentText());
    settings.setValue("qemuMonitorPort", this->m_monitorSocketSpinBox->value());
#endif

    // Update
    settings.setValue("update", this->m_updateCheckBox->isChecked());
    settings.setValue("release", this->m_releaseString);

    // Language
    settings.setValue("language", this->m_languageISOCode);
    settings.setValue("languagePos", this->m_languagePos);

    // Start page
    // QtEmu 2.x
    //settings.setValue("startCommand", this->m_beforeStart->toPlainText());
    //settings.setValue("afterCommand", this->m_afterExit->toPlainText());

    // Proxy
    settings.setValue("proxyType", this->m_proxyOptions->currentIndex());
    settings.setValue("proxyHostname", this->m_serverNameProxy->text());
    settings.setValue("proxyPort", this->m_portProxy->text());
    settings.setValue("auth", this->m_useAuth->isChecked());
    settings.setValue("proxyUser", this->m_userProxy->text());
    settings.setValue("proxyPassword", this->m_passwordProxy->text().toUtf8().toBase64());

    // QEMU
    settings.setValue("qemuBinaryPath", this->m_binaryPathLineEdit->text());
    settings.setValue("qemuImgBinaryPath", this->m_QEMUImgPathLineEdit->text());

    settings.endGroup();
    settings.sync();

    this->hide();

    qDebug() << "ConfigWindow: settings saved";
}

/**
 * @brief Load the QtEmu settings
 *
 * Load the QtEmu settings
 */
void ConfigWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Configuration");

    // General
    this->m_machinePathLineEdit->setText(settings.value("machinePath", QDir::homePath()).toString());
#ifdef Q_OS_WIN
    this->m_monitorHostnameComboBox->setCurrentText(settings.value("qemuMonitorHost", "localhost").toString());
    this->m_monitorSocketSpinBox->setValue(settings.value("qemuMonitorPort", 6000).toInt());
#endif
    // Update
    this->m_updateCheckBox->setChecked(settings.value("update", true).toBool());
    this->m_releaseString = settings.value("release", "stable").toString();

    if (this->m_releaseString == "alpha") {
        this->m_stableReleaseRadio->setChecked(true);
    } else if (this->m_releaseString == "beta") {
        this->m_betaReleaseRadio->setChecked(true);
    } else {
        this->m_stableReleaseRadio->setChecked(true);
    }

    // Language
    this->m_languagesListView->setCurrentRow(settings.value("languagePos", 0).toInt());

    // Start page
    // QtEmu 2.x
    //this->m_beforeStart->setPlainText(settings.value("startCommand", "").toString());
    //this->m_afterExit->setPlainText(settings.value("afterCommand", "").toString());

    // Proxy
    this->m_proxyOptions->setCurrentIndex(settings.value("proxyType", 0).toInt());
    this->m_serverNameProxy->setText(settings.value("proxyHostname", "").toString());
    this->m_portProxy->setText(settings.value("proxyPort", "").toString());
    this->m_useAuth->setChecked(settings.value("auth", false).toBool());
    this->m_userProxy->setText(settings.value("proxyUser", "").toString());
    this->m_passwordProxy->setText(QByteArray::fromBase64(settings.value("proxyPassword").toByteArray()));
    this->toggleServerPort(settings.value("proxyType", 0).toInt());
    this->toggleAuth(this->m_useAuth);

    // QEMU
    this->binaryPathChanged(settings.value("qemuBinaryPath", "").toString());
    this->m_binaryPathLineEdit->setText(settings.value("qemuBinaryPath", "").toString());
    this->m_QEMUImgPathLineEdit->setText(settings.value("qemuImgBinaryPath", "").toString());
    this->insertBinariesInTree();

    settings.endGroup();
}

/**
 * @brief Set the path for QEMU binaries
 *
 * Set the path for QEMU binaries
 */
void ConfigWindow::setPathBinaries()
{
    QString binariesPath = QFileDialog::getExistingDirectory(this, tr("Select a folder for QEMU binaries"),
                                                             QDir::homePath(),
                                                             QFileDialog::ShowDirsOnly |
                                                             QFileDialog::DontResolveSymlinks
                                                             );

    if (!binariesPath.isEmpty()) {
        this->m_binaryPathLineEdit->setText(binariesPath);
        this->m_searchBinariesToolButton->setEnabled(true);
    }
}

/**
 * @brief Find QEMU binaries
 *
 * Find QEMU binaries in the specific folder
 */
void ConfigWindow::findBinaries()
{
    this->m_QEMUObject->setQEMUBinaries(this->m_binaryPathLineEdit->text());
    this->insertBinariesInTree();
}

/**
 * @brief Enable/disable the search binaries button
 *
 * Enable/disable the search binaries button.
 * If the input are empty, the button change to disabled
 * If the input are fill, the button change to enabled
 */
void ConfigWindow::binaryPathChanged(const QString binaryPath)
{
    this->m_binariesTableWidget->clearContents();
    this->m_binariesTableWidget->setRowCount(0);
    this->m_searchBinariesToolButton->setEnabled(!binaryPath.isEmpty());
}

/**
 * @brief Find qemu-img binary
 *
 * Find qemu-img binary in the specific folder
 */
void ConfigWindow::findQemuImgBinary() {
    this->m_QEMUImgPathLineEdit->clear();

    QString qemuImgPath = QFileDialog::getOpenFileName(this,
                                                       tr("Select the qemu-img binary"),
                                                       QDir::homePath()
                                                       );

    if (!qemuImgPath.isEmpty()) {
        this->m_QEMUImgPathLineEdit->setText(qemuImgPath);
    }
}

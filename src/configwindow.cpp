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

// Qt

// Local
#include "configwindow.h"

/**
 * @brief QtEmu configuration menu
 * @param parent, widget parent
 *
 * Configuration of QtEmu. Configure the machines path, language,
 * qemu before and after commands and the proxy.
 */
ConfigWindow::ConfigWindow(QWidget *parent) : QWidget(parent) {
    this -> setWindowTitle(tr("Preferences") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/images/qtemu.png")));
    this -> setWindowFlags(Qt::Dialog);
    this -> setWindowModality(Qt::ApplicationModal);
    this -> setMinimumSize(640, 520);

    QSettings settings;

    settings.beginGroup("Configuration");

    this -> createGeneralPage();

    this -> createUpdatePage();

    this -> createLanguagePage();

    this -> createStartPage();

    this -> createProxyPage();

    settings.endGroup();

    m_optionsListWidget = new QListWidget(this);
    m_optionsListWidget -> setViewMode(QListView::ListMode);
    m_optionsListWidget -> setIconSize(QSize(32, 32));
    m_optionsListWidget -> setMovement(QListView::Static);
    m_optionsListWidget -> setMaximumWidth(170);
    m_optionsListWidget -> setSpacing(7);
    m_optionsListWidget -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add options
    m_optionsListWidget -> addItem(tr("General Options"));
    m_optionsListWidget -> item(0) -> setIcon(QIcon::fromTheme("preferences-other",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));

    m_optionsListWidget -> addItem(tr("Update QtEmu"));
    m_optionsListWidget -> item(1) -> setIcon(QIcon::fromTheme("update-none",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/update-none.svg"))));

    m_optionsListWidget -> addItem(tr("Language"));
    m_optionsListWidget -> item(2) -> setIcon(QIcon::fromTheme("applications-education-language",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/applications-education-language.svg"))));

    m_optionsListWidget -> addItem(tr("Start"));
    m_optionsListWidget -> item(3) -> setIcon(QIcon::fromTheme("practice-start",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/practice-start.svg"))));

    m_optionsListWidget -> addItem(tr("Proxy"));
    m_optionsListWidget -> item(4) -> setIcon(QIcon::fromTheme("network-manager",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    // Prepare window
    m_categoriesStackedWidget = new QStackedWidget(this);
    m_categoriesStackedWidget -> setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::MinimumExpanding);

    m_categoriesStackedWidget -> addWidget(m_generalPageWidget);
    m_categoriesStackedWidget -> addWidget(m_updatePageWidget);
    m_categoriesStackedWidget -> addWidget(m_languagePageWidget);
    m_categoriesStackedWidget -> addWidget(m_startPageWidget);
    m_categoriesStackedWidget -> addWidget(m_proxyPageWidget);

    connect(m_optionsListWidget, &QListWidget::currentRowChanged,
            m_categoriesStackedWidget, &QStackedWidget::setCurrentIndex);

    m_topLayout = new QHBoxLayout();
    m_topLayout -> addWidget(m_optionsListWidget);
    m_topLayout -> addWidget(m_categoriesStackedWidget);

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
            this, &QWidget::hide);

    this -> m_buttonsLayout = new QHBoxLayout();
    m_buttonsLayout -> setAlignment(Qt::AlignRight);
    m_buttonsLayout -> addWidget(m_saveButton);
    m_buttonsLayout -> addWidget(m_closeButton);

    m_closeAction = new QAction(this);
    m_closeAction -> setShortcut(QKeySequence(Qt::Key_Escape));
    connect(m_closeAction, &QAction::triggered, this, &QWidget::hide);
    this -> addAction(m_closeAction);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout -> addLayout(m_topLayout, 20);
    m_mainLayout -> addSpacing(8);
    m_mainLayout -> addStretch(1);
    m_mainLayout -> addLayout(m_buttonsLayout);

    this -> setLayout(m_mainLayout);

    this -> m_optionsListWidget -> setCurrentRow(0);
    this -> m_optionsListWidget -> setFocus();

    // Load settings
    loadSettings();

    qDebug() << "ConfigWindow created";

}

ConfigWindow::~ConfigWindow() {
    qDebug() << "ConfigWindow destroyed";
}

/**
 * @brief Create the general page of the QtEmu configuration
 *
 * Create the general page of the QtEmu configuration where
 * the machines path are indicated.
 */
void ConfigWindow::createGeneralPage() {

    m_machinePathLabel  = new QLabel(tr("Default machine path") + ":");

    m_machinePathLineEdit = new QLineEdit();
    m_machinePathLineEdit -> setEnabled(false);

    m_machinePathButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                          "",
                                          this);

    connect(m_machinePathButton, &QAbstractButton::clicked,
            this, &ConfigWindow::setMachinePath);

    m_machinePathLayout = new QHBoxLayout();
    m_machinePathLayout -> setAlignment(Qt::AlignTop);
    m_machinePathLayout -> addWidget(m_machinePathLabel);
    m_machinePathLayout -> addWidget(m_machinePathLineEdit);
    m_machinePathLayout -> addWidget(m_machinePathButton);

    m_machinePathGroup = new QGroupBox(tr("Machine Path"));
    m_machinePathGroup -> setLayout(m_machinePathLayout);

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout -> addWidget(m_machinePathGroup);

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget -> setLayout(m_generalPageLayout);
}

/**
 * @brief Create the update page of the QtEmu configuration
 *
 * Create the update page of the QtEmu configuration where
 * the version of the software can be selected.
 * Versions: Stable, Beta, Development
 */
void ConfigWindow::createUpdatePage(){

    m_updateCheckBox = new QCheckBox();
    m_updateCheckBox -> setChecked(true);

    connect(m_updateCheckBox, &QAbstractButton::toggled,
                this, &ConfigWindow::toggleUpdate);

    m_updatesGroup = new QGroupBox();

    m_stableReleaseRadio = new QRadioButton(tr("Stable version"));
    m_betaReleaseRadio = new QRadioButton(tr("Beta version"));
    m_developmentRelaseRadio = new QRadioButton(tr("Development version"));

    m_stableReleaseRadio -> setChecked(true);

    connect(m_stableReleaseRadio, &QAbstractButton::toggled,
                this, &ConfigWindow::pushStableVersion);

    m_updateRadiosLayout = new QVBoxLayout();
    m_updateRadiosLayout -> setAlignment(Qt::AlignVCenter);
    m_updateRadiosLayout -> addWidget(m_stableReleaseRadio);
    m_updateRadiosLayout -> addWidget(m_betaReleaseRadio);
    m_updateRadiosLayout -> addWidget(m_developmentRelaseRadio);

    m_updatesGroup -> setLayout(m_updateRadiosLayout);

    m_updatePageLayout = new QFormLayout();
    m_updatePageLayout -> addRow(tr("Check for updates"), m_updateCheckBox);
    m_updatePageLayout -> addRow(m_updatesGroup);

    m_updatePageWidget = new QWidget(this);
    m_updatePageWidget -> setLayout(m_updatePageLayout);
}

/**
 * @brief Create the language page of the QtEmu configuration
 *
 * Create the language page of the QtEmu configuration where
 * the language can be selected.
 */
void ConfigWindow::createLanguagePage(){
    m_languageLabel = new QLabel(tr("Interface language"));

    m_languagesListView = new QListWidget();

    m_languagesListView -> setViewMode(QListView::ListMode);
    m_languagesListView -> setIconSize(QSize(32, 32));
    m_languagesListView -> setMovement(QListView::Static);
    m_languagesListView -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_languagesListView -> addItem("English");
    m_languagesListView -> item(0) -> setIcon(QIcon(":/images/flags/32x32/eeuu.png"));

    m_languageDescription = new QLabel();
    m_languageAuthors = new QLabel();

    connect(m_languagesListView, &QListWidget::currentTextChanged,
            this, &ConfigWindow::setLanguageLabel);

    connect(m_languagesListView, &QListWidget::currentRowChanged,
            this, &ConfigWindow::setAuthorsLabel);

    m_languagePageLayout = new QVBoxLayout();
    m_languagePageLayout -> addWidget(m_languageLabel);
    m_languagePageLayout -> addWidget(m_languagesListView);

    m_languageHLayout = new QHBoxLayout();
    m_languageHLayout -> addWidget(m_languageDescription);
    m_languageHLayout -> addWidget(m_languageAuthors);

    m_languagePageLayout -> addItem(m_languageHLayout);

    m_languagePageWidget = new QWidget(this);
    m_languagePageWidget -> setLayout(m_languagePageLayout);
}

/**
 * @brief Create the start page of the QtEmu configuration
 *
 * Create the start page of the QtEmu configuration where
 * can be configured wich commands execute before and after
 * the launch of QEMU.
 */
void ConfigWindow::createStartPage(){

    m_beforeStartLabel = new QLabel(tr("Execute before start") + ":");
    m_afterExitLabel = new QLabel(tr("Execute after exit") + ":");

    m_beforeStart = new QPlainTextEdit();
    m_afterExit = new QPlainTextEdit();

    m_startPageLayout = new QVBoxLayout();

    m_startPageLayout -> addWidget(m_beforeStartLabel);
    m_startPageLayout -> addWidget(m_beforeStart);
    m_startPageLayout -> addWidget(m_afterExitLabel);
    m_startPageLayout -> addWidget(m_afterExit);

    m_startPageWidget = new QWidget(this);
    m_startPageWidget -> setLayout(m_startPageLayout);
}

/**
 * @brief Create the proxy page of the QtEmu configuration
 *
 * Create the proxy page of the QtEmu configuration where the
 * proxy options can be configured.
 */
void ConfigWindow::createProxyPage(){

    m_proxyOptions = new QComboBox();
    m_proxyOptions -> addItem(tr("Do not use a proxy"));
    m_proxyOptions -> addItem("SOCKS 5");
    m_proxyOptions -> addItem("HTTP");
    m_proxyOptions -> setCurrentIndex(0);

    m_serverNameProxy = new QLineEdit();
    m_serverNameProxy -> setPlaceholderText("example.org");

    m_portProxy = new QLineEdit();
    m_portProxy -> setPlaceholderText("1080, 8080, etc...");

    connect(m_proxyOptions, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ConfigWindow::toggleServerPort);

    m_useAuth = new QCheckBox();
    m_useAuth -> setChecked(false);

    connect(m_useAuth, &QAbstractButton::toggled,
                this, &ConfigWindow::toggleAuth);

    m_userProxy = new QLineEdit();
    m_userProxy -> setPlaceholderText(tr("Proxy username"));

    m_passwordProxy = new QLineEdit();
    m_passwordProxy -> setEchoMode(QLineEdit::Password);

    // TODO: read the user settings
    this -> toggleServerPort(0);
    this -> toggleAuth(false);

    m_proxyPageLayout = new QFormLayout();

    m_proxyPageLayout -> addRow(tr("Proxy type"), m_proxyOptions);
    m_proxyPageLayout -> addRow(tr("Hostname"), m_serverNameProxy);
    m_proxyPageLayout -> addRow(tr("Port"), m_portProxy);
    m_proxyPageLayout -> addRow(tr("Use Authentication"), m_useAuth);
    m_proxyPageLayout -> addRow(tr("User"), m_userProxy);
    m_proxyPageLayout -> addRow(tr("Password"), m_passwordProxy);

    m_proxyPageWidget = new QWidget(this);
    m_proxyPageWidget -> setLayout(m_proxyPageLayout);
}

/**
 * @brief Enable or disabled the update radios
 * @param updateState
 *
 * Indicate if the radios referred to the update section are enabled or disabled
 */
void ConfigWindow::toggleUpdate(bool updateState) {

    this -> m_updatesGroup -> setEnabled(updateState);
    this -> m_stableReleaseRadio -> setEnabled(updateState);
    this -> m_betaReleaseRadio -> setEnabled(updateState);
    this -> m_developmentRelaseRadio -> setEnabled(updateState);

}

/**
 * @brief Select stable version
 * @param release
 *
 * Select stable version for the QtEmu release chanel
 */
void ConfigWindow::pushStableVersion(bool release){

    if (release) {
        m_releaseString = "stable";
    }

}

/**
 * @brief Select beta version
 * @param release
 *
 * Select beta version for the QtEmu release chanel
 */
void ConfigWindow::pushBetaVersion(bool release){

    if (release) {
        m_releaseString = "beta";
    }

}

/**
 * @brief Select development version
 * @param release
 *
 * Select development version for the QtEmu release chanel
 */
void ConfigWindow::pushDevelopmentVersion(bool release){

    if (release) {
        m_releaseString = "alpha";
    }

}

/**
 * @brief Set the language label
 * @param language, language string
 *
 * Set the language label
 */
void ConfigWindow::setLanguageLabel(QString language){
    QString description = tr("Language") + ": ";
    description.append(language);

    this -> m_languageDescription -> setText(description);
}

/**
 * @brief Set the name of the translator
 * @param languagePosition, position of the language
 *
 * Set the name of the translator
 */
void ConfigWindow::setAuthorsLabel(int languagePosition){
    QString authors = tr("Authors") + ": ";

    switch (languagePosition) {
        case 0:
            authors.append(tr("QtEmu Developers"));
            m_languageISOCode = "en";
            m_languagePos = 0;
            break;
        default:
            authors.append(tr("Unknown author"));
            m_languageISOCode = "en";
            m_languagePos = 0;
            break;
    }

    this -> m_languageAuthors -> setText(authors);
}

/**
 * @brief Activate or deactivate the server and port
 * @param proxyOption
 *
 * Activate or deactivate the server and port for
 * QtEmu
 */
void ConfigWindow::toggleServerPort(int proxyOption){

    if (proxyOption == 0) {
        this -> m_serverNameProxy -> setEnabled(false);
        this -> m_portProxy -> setEnabled(false);
    } else {
        this -> m_serverNameProxy -> setEnabled(true);
        this -> m_portProxy -> setEnabled(true);
    }
}

/**
 * @brief Establish user and password for proxy settings
 * @param authState, activate or deactivate the user and password inputs
 *
 * Establish user and password for proxy settings
 */
void ConfigWindow::toggleAuth(bool authState){

    if (!authState) {
        this -> m_userProxy -> setText("");
        this -> m_passwordProxy -> setText("");
    }

    this -> m_userProxy -> setEnabled(authState);
    this -> m_passwordProxy -> setEnabled(authState);

}

/**
 * @brief Set the path for the machines
 *
 * Establish the path where the machines are
 * going to be located
 */
void ConfigWindow::setMachinePath(){

    m_machinePath = QFileDialog::getExistingDirectory(this, tr("Select a folder for Machines"),
                                                      QDir::homePath(),
                                                      QFileDialog::ShowDirsOnly |
                                                      QFileDialog::DontResolveSymlinks
                                                      );
    if ( !m_machinePath.isEmpty() ) {
        m_machinePathLineEdit -> setText(QDir::toNativeSeparators(m_machinePath));
    }

}

/**
 * @brief Save the QtEmu settings
 *
 * Load the QtEmu settings
 */
void ConfigWindow::saveSettings(){
    QSettings settings;

    if (!settings.isWritable()) {
        return;
    }

    settings.beginGroup("Configuration");

    // General
    settings.setValue("machinePath", this -> m_machinePathLineEdit -> text());

    // Update
    settings.setValue("update", this -> m_updateCheckBox -> isChecked());
    settings.setValue("release", this -> m_releaseString);

    // Language
    settings.setValue("language", this -> m_languageISOCode);
    settings.setValue("languagePos", this -> m_languagePos);

    // Start page
    settings.setValue("startCommand", this -> m_beforeStart -> toPlainText());
    settings.setValue("afterCommand", this -> m_afterExit -> toPlainText());

    // Proxy
    settings.setValue("proxyType", this -> m_proxyOptions -> currentIndex());
    settings.setValue("proxyHostname", this -> m_serverNameProxy -> text());
    settings.setValue("proxyPort", this -> m_portProxy -> text());
    settings.setValue("auth", this -> m_useAuth -> isChecked());
    settings.setValue("proxyUser", this -> m_userProxy -> text());
    settings.setValue("proxyPassword", this -> m_passwordProxy -> text().toUtf8().toBase64());

    settings.endGroup();
    settings.sync();

    this -> hide();

    qDebug() << "ConfigWindow: settings saved";

}

/**
 * @brief Load the QtEmu settings
 *
 * Load the QtEmu settings
 */
void ConfigWindow::loadSettings(){
    QSettings settings;
    settings.beginGroup("Configuration");

    // General
    m_machinePathLineEdit -> setText(settings.value("machinePath", QDir::homePath()).toString());

    // Update
    m_updateCheckBox -> setChecked(settings.value("update", true).toBool());
    m_releaseString = settings.value("release", "stable").toString();

    // Language
    m_languagesListView -> setCurrentRow(settings.value("languagePos", 0).toInt());

    // Start page
    m_beforeStart -> setPlainText(settings.value("startCommand", "").toString());
    m_afterExit -> setPlainText(settings.value("afterCommand", "").toString());

    // Proxy
    m_proxyOptions -> setCurrentIndex(settings.value("proxyType", 0).toInt());
    m_serverNameProxy -> setText(settings.value("proxyHostname", "").toString());
    m_portProxy -> setText(settings.value("proxyPort", "").toString());
    m_useAuth -> setChecked(settings.value("auth", false).toBool());
    m_userProxy -> setText(settings.value("proxyUser", "").toString());
    m_passwordProxy -> setText(QByteArray::fromBase64(settings.value("proxyPassword").toByteArray()));

    settings.endGroup();
}

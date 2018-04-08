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

ConfigWindow::ConfigWindow(QWidget *parent) : QWidget(parent) {
    this -> setWindowTitle(tr("Preferences") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/icon/32x32/qtemu.png")));
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::ApplicationModal);
    this->setMinimumSize(640, 520);

    QSettings settings;

    settings.beginGroup("Configuration");

    this -> createGeneralPage();

    // TODO
    //this -> createInputPage();

    this -> createUpdatePage();

    this -> createLanguagePage();

    this -> createStartPage();

    this -> createProxyPage();

    settings.endGroup();

    optionsListWidget = new QListWidget(this);
    optionsListWidget -> setViewMode(QListView::ListMode);
    optionsListWidget -> setIconSize(QSize(32, 32));
    optionsListWidget -> setMovement(QListView::Static);
    optionsListWidget -> setMaximumWidth(170);
    optionsListWidget -> setSpacing(7);
    optionsListWidget -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add options
    optionsListWidget -> addItem(tr("General Options"));
    optionsListWidget -> item(0) -> setIcon(QIcon::fromTheme("preferences-other",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    //optionsListWidget -> addItem(tr("Input"));
    //optionsListWidget -> item(1) -> setIcon(QIcon::fromTheme("preferences-desktop-keyboard",
    //                                                          QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Update QtEmu"));
    optionsListWidget -> item(1) -> setIcon(QIcon::fromTheme("update-none",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Language"));
    optionsListWidget -> item(2) -> setIcon(QIcon::fromTheme("applications-education-language",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Start"));
    optionsListWidget -> item(3) -> setIcon(QIcon::fromTheme("practice-start",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Proxy"));
    optionsListWidget -> item(4) -> setIcon(QIcon::fromTheme("network-manager",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    // Prepare window
    categoriesStackedWidget = new QStackedWidget(this);
    categoriesStackedWidget -> setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::MinimumExpanding);

    categoriesStackedWidget -> addWidget(generalPageWidget);
    //categoriesStackedWidget -> addWidget(inputPageWidget);
    categoriesStackedWidget -> addWidget(updatePageWidget);
    categoriesStackedWidget -> addWidget(languagePageWidget);
    categoriesStackedWidget -> addWidget(startPageWidget);
    categoriesStackedWidget -> addWidget(proxyPageWidget);

    connect(optionsListWidget, &QListWidget::currentRowChanged,
            categoriesStackedWidget, &QStackedWidget::setCurrentIndex);

    topLayout = new QHBoxLayout();
    topLayout -> addWidget(optionsListWidget);
    topLayout -> addWidget(categoriesStackedWidget);

    // Buttons
    saveButton = new QPushButton(QIcon::fromTheme("document-save",
                                                  QIcon(":/icon/32x32/qtemu.png")),
                                 tr("Save"),
                                 this);
    connect(saveButton, &QAbstractButton::clicked,
            this, &ConfigWindow::saveSettings);

    closeButton = new QPushButton(QIcon::fromTheme("dialog-cancel",
                                                   QIcon(":/icon/32x32/qtemu.png")),
                                  tr("Cancel"),
                                  this);
    connect(closeButton, &QAbstractButton::clicked,
            this, &QWidget::hide);

    this -> buttonsLayout = new QHBoxLayout();
    buttonsLayout -> setAlignment(Qt::AlignRight);
    buttonsLayout -> addWidget(saveButton);
    buttonsLayout -> addWidget(closeButton);

    closeAction = new QAction(this);
    closeAction -> setShortcut(QKeySequence(Qt::Key_Escape));
    connect(closeAction, &QAction::triggered, this, &QWidget::hide);
    this -> addAction(closeAction);

    mainLayout = new QVBoxLayout();
    mainLayout -> addLayout(topLayout, 20);
    mainLayout -> addSpacing(8);
    mainLayout -> addStretch(1);
    mainLayout -> addLayout(buttonsLayout);

    this -> setLayout(mainLayout);

    this -> optionsListWidget -> setCurrentRow(0);
    this -> optionsListWidget -> setFocus();

    // Load settings
    loadSettings();

    qDebug() << "ConfigWindow created";

}

ConfigWindow::~ConfigWindow() {
    qDebug() << "ConfigWindow destroyed";
}

void ConfigWindow::createGeneralPage() {

    machinePathGroup = new QGroupBox(tr("Machine Path"));

    machinePathLabel  = new QLabel(tr("Default machine path") + ":");

    machinePathLineEdit = new QLineEdit();
    machinePathLineEdit -> setEnabled(false);

    machinePathButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                         QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    connect(machinePathButton, &QAbstractButton::clicked,
            this, &ConfigWindow::setMachinePath);

    machinePathLayout = new QHBoxLayout();
    machinePathLayout -> setAlignment(Qt::AlignVCenter);
    machinePathLayout -> addWidget(machinePathLabel);
    machinePathLayout -> addWidget(machinePathLineEdit);
    machinePathLayout -> addWidget(machinePathButton);

    machinePathGroup -> setLayout(machinePathLayout);

    startCommandGroup = new QGroupBox(tr("QEMU Command"));

    startCommandLabel = new QLabel(tr("QEMU start command") + ":");

    startCommandLineEdit = new QLineEdit();
    startCommandLineEdit -> setText("qemu"); // TODO: add support for MacOS and Windows

    startCommandLayout = new QHBoxLayout();
    startCommandLayout -> setAlignment(Qt::AlignVCenter);
    startCommandLayout -> addWidget(startCommandLabel);
    startCommandLayout -> addWidget(startCommandLineEdit);

    startCommandGroup -> setLayout(startCommandLayout);

    generalPageLayout = new QVBoxLayout();
    generalPageLayout -> addWidget(machinePathGroup);
    generalPageLayout -> addWidget(startCommandGroup);

    generalPageWidget = new QWidget(this);
    generalPageWidget -> setLayout(generalPageLayout);
}

void ConfigWindow::createInputPage(){

    inputPageLayout = new QVBoxLayout();

    inputPageWidget = new QWidget(this);
    inputPageWidget -> setLayout(inputPageLayout);
}

void ConfigWindow::createUpdatePage(){

    updateCheckBox = new QCheckBox();
    updateCheckBox -> setChecked(true);

    connect(updateCheckBox, &QAbstractButton::toggled,
                this, &ConfigWindow::toggleUpdate);

    updatesGroup = new QGroupBox();

    stableReleaseRadio = new QRadioButton(tr("Stable version"));
    betaReleaseRadio = new QRadioButton(tr("Beta version"));
    developmentRelaseRadio = new QRadioButton(tr("Development version"));

    stableReleaseRadio -> setChecked(true);

    connect(stableReleaseRadio, &QAbstractButton::toggled,
                this, &ConfigWindow::pushStableVersion);

    updateRadiosLayout = new QVBoxLayout();
    updateRadiosLayout -> setAlignment(Qt::AlignVCenter);
    updateRadiosLayout -> addWidget(stableReleaseRadio);
    updateRadiosLayout -> addWidget(betaReleaseRadio);
    updateRadiosLayout -> addWidget(developmentRelaseRadio);

    updatesGroup -> setLayout(updateRadiosLayout);

    updatePageLayout = new QFormLayout();
    updatePageLayout -> addRow(tr("Check for updates"), updateCheckBox);
    updatePageLayout -> addRow(updatesGroup);

    updatePageWidget = new QWidget(this);
    updatePageWidget -> setLayout(updatePageLayout);
}

void ConfigWindow::createLanguagePage(){
    languageLabel = new QLabel(tr("Interface language"));

    languagesListView = new QListWidget();

    languagesListView -> setViewMode(QListView::ListMode);
    languagesListView -> setIconSize(QSize(32, 32));
    languagesListView -> setMovement(QListView::Static);
    languagesListView -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    languagesListView -> addItem("English");
    languagesListView -> item(0) -> setIcon(QIcon::fromTheme("eeuu-flag",
                                                              QIcon(":/icon/32x32/flags/eeuu.png")));

    languagesListView -> addItem("Deutsch");
    languagesListView -> item(1) -> setIcon(QIcon::fromTheme("germany-flag",
                                                              QIcon(":/icon/32x32/flags/germany.png")));

    languagesListView -> addItem(QString::fromUtf8("Türkçe"));
    languagesListView -> item(2) -> setIcon(QIcon::fromTheme("turkey-flag",
                                                              QIcon(":/icon/32x32/flags/turkey.png")));

    languagesListView -> addItem(QString::fromUtf8("Русский"));
    languagesListView -> item(3) -> setIcon(QIcon::fromTheme("russia-flag",
                                                              QIcon(":/icon/32x32/flags/russia.png")));

    languagesListView -> addItem(QString::fromUtf8("Česky"));
    languagesListView -> item(4) -> setIcon(QIcon::fromTheme("czech-flag",
                                                              QIcon(":/icon/32x32/flags/czech.png")));

    languagesListView -> addItem(QString::fromUtf8("Español"));
    languagesListView -> item(5) -> setIcon(QIcon::fromTheme("spain-flag",
                                                              QIcon(":/icon/32x32/flags/spain.png")));

    languagesListView -> addItem(QString::fromUtf8("Français"));
    languagesListView -> item(6) -> setIcon(QIcon::fromTheme("france-flag",
                                                              QIcon(":/icon/32x32/flags/france.png")));

    languagesListView -> addItem(QString::fromUtf8("Italiano"));
    languagesListView -> item(7) -> setIcon(QIcon::fromTheme("italy-flag",
                                                              QIcon(":/icon/32x32/flags/italy.png")));

    languagesListView -> addItem(QString::fromUtf8("Português do Brasil"));
    languagesListView -> item(8) -> setIcon(QIcon::fromTheme("brazil-flag",
                                                              QIcon(":/icon/32x32/flags/brazil.png")));

    languagesListView -> addItem(QString::fromUtf8("Polski"));
    languagesListView -> item(9) -> setIcon(QIcon::fromTheme("poland-flag",
                                                              QIcon(":/icon/32x32/flags/poland.png")));

    languageDescription = new QLabel();
    languageAuthors = new QLabel();

    connect(languagesListView, &QListWidget::currentTextChanged,
            this, &ConfigWindow::setLanguageLabel);

    connect(languagesListView, &QListWidget::currentRowChanged,
            this, &ConfigWindow::setAuthorsLabel);

    languagePageLayout = new QVBoxLayout();
    languagePageLayout -> addWidget(languageLabel);
    languagePageLayout -> addWidget(languagesListView);

    languageHLayout = new QHBoxLayout();
    languageHLayout -> addWidget(languageDescription);
    languageHLayout -> addWidget(languageAuthors);

    languagePageLayout -> addItem(languageHLayout);

    languagePageWidget = new QWidget(this);
    languagePageWidget -> setLayout(languagePageLayout);
}

void ConfigWindow::createStartPage(){

    beforeStartLabel = new QLabel(tr("Execute before start") + ":");
    afterExitLabel = new QLabel(tr("Execute after exit") + ":");

    beforeStart = new QPlainTextEdit();
    afterExit = new QPlainTextEdit();

    startPageLayout = new QVBoxLayout();

    startPageLayout -> addWidget(beforeStartLabel);
    startPageLayout -> addWidget(beforeStart);
    startPageLayout -> addWidget(afterExitLabel);
    startPageLayout -> addWidget(afterExit);

    startPageWidget = new QWidget(this);
    startPageWidget -> setLayout(startPageLayout);
}

void ConfigWindow::createProxyPage(){

    proxyOptions = new QComboBox();
    proxyOptions -> addItem(tr("Do not use a proxy"));
    proxyOptions -> addItem("SOCKS 5");
    proxyOptions -> addItem("HTTP");
    proxyOptions -> setCurrentIndex(0);

    serverNameProxy = new QLineEdit();
    serverNameProxy -> setPlaceholderText("example.org");

    portProxy = new QLineEdit();
    portProxy -> setPlaceholderText("1080, 8080, etc...");

    connect(proxyOptions, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ConfigWindow::toggleUserPassword);

    useAuth = new QCheckBox();
    useAuth -> setChecked(false);

    connect(useAuth, &QAbstractButton::toggled,
                this, &ConfigWindow::toggleAuth);

    userProxy = new QLineEdit();
    userProxy -> setPlaceholderText(tr("Proxy username"));

    passwordProxy = new QLineEdit();
    passwordProxy -> setEchoMode(QLineEdit::Password);

    // TODO: read the user settings
    this -> toggleUserPassword(0);
    this -> toggleAuth(false);

    proxyPageLayout = new QFormLayout();

    proxyPageLayout -> addRow(tr("Proxy type"), proxyOptions);
    proxyPageLayout -> addRow(tr("Hostname"), serverNameProxy);
    proxyPageLayout -> addRow(tr("Port"), portProxy);
    proxyPageLayout -> addRow(tr("Use Authentication"), useAuth);
    proxyPageLayout -> addRow(tr("User"), userProxy);
    proxyPageLayout -> addRow(tr("Password"), passwordProxy);

    proxyPageWidget = new QWidget(this);
    proxyPageWidget -> setLayout(proxyPageLayout);
}

void ConfigWindow::toggleUpdate(bool updateState){

    this -> updatesGroup -> setEnabled(updateState);
    this -> stableReleaseRadio -> setEnabled(updateState);
    this -> betaReleaseRadio -> setEnabled(updateState);
    this -> developmentRelaseRadio -> setEnabled(updateState);

}

void ConfigWindow::pushStableVersion(bool release){

    if (release) {
        releaseString = "stable";
    }

}

void ConfigWindow::pushBetaVersion(bool release){

    if (release) {
        releaseString = "beta";
    }

}

void ConfigWindow::pushDevelopmentVersion(bool release){

    if (release) {
        releaseString = "alpha";
    }

}

void ConfigWindow::setLanguageLabel(QString language){
    QString description = tr("Language") + ": ";
    description.append(language);

    this -> languageDescription -> setText(description);
}

void ConfigWindow::setAuthorsLabel(int languagePosition){
    QString authors = tr("Authors") + ": ";

    switch (languagePosition) {
        case 0:
            authors.append(tr("QtEmu Developers"));
            languageISOCode = "en";
            languagePos = 0;
            break;
        case 1:
            authors.append(tr("QtEmu Developers"));
            languageISOCode = "de";
            languagePos = 1;
            break;
        case 2:
            authors.append(QString::fromUtf8("Necmettin Begiter"));
            languageISOCode = "tr";
            languagePos = 2;
            break;
        case 3:
            authors.append(QString::fromUtf8("Vasily Cheremisinov"));
            languageISOCode = "ru";
            languagePos = 3;
            break;
        case 4:
            authors.append(QString::fromUtf8("excamo"));
            languageISOCode = "cz";
            languagePos = 4;
            break;
        case 5:
            authors.append(QString::fromUtf8("Manolo Valdes"));
            languageISOCode = "es";
            languagePos = 5;
            break;
        case 6:
            authors.append(QString::fromUtf8("Fathi Boudra"));
            languageISOCode = "fr";
            languagePos = 6;
            break;
        case 7:
            authors.append(QString::fromUtf8("Gianluca Cecchi"));
            languageISOCode = "it";
            languagePos = 7;
            break;
        case 8:
            authors.append(QString::fromUtf8("Jackson Miliszewski"));
            languageISOCode = "pt-BR";
            languagePos = 8;
            break;
        case 9:
            authors.append(QString::fromUtf8("Milosz Galazka"));
            languageISOCode = "pl";
            languagePos = 9;
            break;
        default:
            authors.append(tr("Unknown author"));
            languageISOCode = "en";
            languagePos = 0;
            break;
    }

    this -> languageAuthors -> setText(authors);
}

void ConfigWindow::toggleUserPassword(int proxyOption){

    if (proxyOption == 0) {
        this -> serverNameProxy -> setEnabled(false);
        this -> portProxy -> setEnabled(false);
    } else {
        this -> serverNameProxy -> setEnabled(true);
        this -> portProxy -> setEnabled(true);
    }

}

void ConfigWindow::toggleAuth(bool authState){

    if (!authState) {
        this -> userProxy -> setText("");
        this -> passwordProxy -> setText("");
    }

    this -> userProxy -> setEnabled(authState);
    this -> passwordProxy -> setEnabled(authState);

}

void ConfigWindow::setMachinePath(){

    machinePath = QFileDialog::getExistingDirectory(this, tr("Select a folder for Machines"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks
                                                    );
    if ( !machinePath.isEmpty() ) {
        machinePathLineEdit -> setText(QDir::toNativeSeparators(machinePath));
    }

}

void ConfigWindow::saveSettings(){
    QSettings settings;

    if (!settings.isWritable()) {
        return;
    }

    settings.beginGroup("Configuration");

    // General
    settings.setValue("machinePath", this -> machinePathLineEdit -> text());
    settings.setValue("qemu", this -> startCommandLineEdit -> text());

    // Update
    settings.setValue("update", this -> updateCheckBox -> isChecked());
    settings.setValue("release", this -> releaseString);

    // Language
    settings.setValue("language", this -> languageISOCode);
    settings.setValue("languagePos", this -> languagePos);

    // Start page
    settings.setValue("startCommand", this -> beforeStart -> toPlainText());
    settings.setValue("afterCommand", this -> afterExit -> toPlainText());

    // Proxy
    settings.setValue("proxyType", this -> proxyOptions -> currentIndex());
    settings.setValue("proxyHostname", this -> serverNameProxy -> text());
    settings.setValue("proxyPort", this -> portProxy -> text());
    settings.setValue("auth", this -> useAuth -> isChecked());
    settings.setValue("proxyUser", this -> userProxy -> text());
    settings.setValue("proxyPassword", this -> passwordProxy -> text().toUtf8().toBase64());

    settings.endGroup();
    settings.sync();

    this -> hide();

    qDebug() << "ConfigWindow: settings saved";

}

void ConfigWindow::loadSettings(){
    QSettings settings;
    settings.beginGroup("Configuration");

    // General
    machinePathLineEdit -> setText(settings.value("machinePath", QDir::homePath()).toString());
    startCommandLineEdit -> setText(settings.value("qemu", "qemu").toString());

    // Update
    updateCheckBox -> setChecked(settings.value("update", true).toBool());
    releaseString = settings.value("release", "stable").toString();

    // Language
    languagesListView -> setCurrentRow(settings.value("languagePos", 0).toInt());

    // Start page
    beforeStart -> setPlainText(settings.value("startCommand", "").toString());
    afterExit -> setPlainText(settings.value("afterCommand", "").toString());

    // Proxy
    proxyOptions -> setCurrentIndex(settings.value("proxyType", 0).toInt());
    serverNameProxy -> setText(settings.value("proxyHostname", "").toString());
    portProxy -> setText(settings.value("proxyPort", "").toString());
    useAuth -> setChecked(settings.value("auth", false).toBool());
    userProxy -> setText(settings.value("proxyUser", "").toString());
    passwordProxy -> setText(QByteArray::fromBase64(settings.value("proxyPassword").toByteArray()));

    settings.endGroup();
}

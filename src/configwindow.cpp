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

    this -> createInputPage();

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

    optionsListWidget -> addItem(tr("Input"));
    optionsListWidget -> item(1) -> setIcon(QIcon::fromTheme("preferences-desktop-keyboard",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Update QtEmu"));
    optionsListWidget -> item(2) -> setIcon(QIcon::fromTheme("update-none",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Language"));
    optionsListWidget -> item(3) -> setIcon(QIcon::fromTheme("applications-education-language",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Start"));
    optionsListWidget -> item(4) -> setIcon(QIcon::fromTheme("practice-start",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    optionsListWidget -> addItem(tr("Proxy"));
    optionsListWidget -> item(5) -> setIcon(QIcon::fromTheme("network-manager",
                                                              QIcon(":/icon/32x32/qtemu.png")));

    // Prepare window
    categoriesStackedWidget = new QStackedWidget(this);
    categoriesStackedWidget -> setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::MinimumExpanding);

    categoriesStackedWidget -> addWidget(generalPageWidget);
    categoriesStackedWidget -> addWidget(inputPageWidget);
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
            this, &QWidget::hide); // TODO Method for save config options

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

    qDebug() << "ConfigWindow created";

}

ConfigWindow::~ConfigWindow() {
    qDebug() << "ConfigWindow destroyed";
}

void ConfigWindow::createGeneralPage() {
    defaultMachineFolder = new QLabel(tr("Default Machine Folder"), this);

    generalPageLayout = new QVBoxLayout();
    generalPageLayout -> addWidget(defaultMachineFolder);

    generalPageWidget = new QWidget(this);
    generalPageWidget -> setLayout(generalPageLayout);
}

void ConfigWindow::createInputPage(){

    inputPageLayout = new QVBoxLayout();

    inputPageWidget = new QWidget(this);
    inputPageWidget -> setLayout(inputPageLayout);
}

void ConfigWindow::createUpdatePage(){

    updatePageLayout = new QVBoxLayout();

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

    proxyPageLayout = new QVBoxLayout();

    proxyPageWidget = new QWidget(this);
    proxyPageWidget -> setLayout(proxyPageLayout);
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
            break;
        case 1:
            authors.append(tr("QtEmu Developers"));
            break;
        case 2:
            authors.append(QString::fromUtf8("Necmettin Begiter"));
            break;
        case 3:
            authors.append(QString::fromUtf8("Vasily Cheremisinov"));
            break;
        case 4:
            authors.append(QString::fromUtf8("excamo"));
            break;
        case 5:
            authors.append(QString::fromUtf8("Manolo Valdes"));
            break;
        case 6:
            authors.append(QString::fromUtf8("Fathi Boudra"));
            break;
        case 7:
            authors.append(QString::fromUtf8("Gianluca Cecchi"));
            break;
        case 8:
            authors.append(QString::fromUtf8("Jackson Miliszewski"));
            break;
        case 9:
            authors.append(QString::fromUtf8("Milosz Galazka"));
            break;
        default:
            authors.append(tr("Unknown author"));
            break;
    }

    this -> languageAuthors -> setText(authors);
}

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
                                                              QIcon(":/images/flags/eeuu.svg")));

    languagesListView -> addItem("Deutsch");
    languagesListView -> item(1) -> setIcon(QIcon::fromTheme("germany-flag",
                                                              QIcon(":/images/flags/germany.svg")));

    languagesListView -> addItem(QString::fromUtf8("Türkçe"));
    languagesListView -> item(2) -> setIcon(QIcon::fromTheme("turkey-flag",
                                                              QIcon(":/images/flags/turkey.svg")));

    languagesListView -> addItem(QString::fromUtf8("Русский"));
    languagesListView -> item(3) -> setIcon(QIcon::fromTheme("russia-flag",
                                                              QIcon(":/images/flags/russia.svg")));

    languagesListView -> addItem(QString::fromUtf8("Česky"));
    languagesListView -> item(4) -> setIcon(QIcon::fromTheme("czech-flag",
                                                              QIcon(":/images/flags/czech.svg")));

    languagesListView -> addItem(QString::fromUtf8("Español"));
    languagesListView -> item(5) -> setIcon(QIcon::fromTheme("spain-flag",
                                                              QIcon(":/images/flags/spain.svg")));

    languagesListView -> addItem(QString::fromUtf8("Français"));
    languagesListView -> item(6) -> setIcon(QIcon::fromTheme("france-flag",
                                                              QIcon(":/images/flags/france.svg")));

    languagesListView -> addItem(QString::fromUtf8("Italiano"));
    languagesListView -> item(7) -> setIcon(QIcon::fromTheme("italy-flag",
                                                              QIcon(":/images/flags/italy.svg")));

    languagesListView -> addItem(QString::fromUtf8("Português do Brasil"));
    languagesListView -> item(8) -> setIcon(QIcon::fromTheme("brazil-flag",
                                                              QIcon(":/images/flags/brazil.svg")));

    languagesListView -> addItem(QString::fromUtf8("Polski"));
    languagesListView -> item(9) -> setIcon(QIcon::fromTheme("poland-flag",
                                                              QIcon(":/images/flags/poland.svg")));

    languagePageLayout = new QVBoxLayout();
    languagePageLayout -> addWidget(languageLabel);
    languagePageLayout -> addWidget(languagesListView);

    languagePageWidget = new QWidget(this);
    languagePageWidget -> setLayout(languagePageLayout);
}

void ConfigWindow::createStartPage(){

    startPageLayout = new QVBoxLayout();

    startPageWidget = new QWidget(this);
    startPageWidget -> setLayout(startPageLayout);
}

void ConfigWindow::createProxyPage(){

    proxyPageLayout = new QVBoxLayout();

    proxyPageWidget = new QWidget(this);
    proxyPageWidget -> setLayout(proxyPageLayout);
}

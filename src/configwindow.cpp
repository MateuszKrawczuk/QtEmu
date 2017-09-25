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

}

void ConfigWindow::createUpdatePage(){

}

void ConfigWindow::createLanguagePage(){

}

void ConfigWindow::createStartPage(){

}

void ConfigWindow::createProxyPage(){

}

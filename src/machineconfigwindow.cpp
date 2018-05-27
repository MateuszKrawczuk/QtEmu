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
#include "machineconfigwindow.h"

MachineConfigWindow::MachineConfigWindow(QUuid machineUuid,
                                         Machine::States machineState,
                                         QWidget *parent) : QWidget(parent) {

    this -> setWindowTitle(tr("Machine Preferences") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/images/qtemu.png")));
    this -> setWindowFlags(Qt::Dialog);
    this -> setWindowModality(Qt::ApplicationModal);
    this -> setMinimumSize(640, 520);

    this -> createGeneralPage();

    m_optionsListWidget = new QListWidget(this);
    m_optionsListWidget -> setViewMode(QListView::ListMode);
    m_optionsListWidget -> setIconSize(QSize(32, 32));
    m_optionsListWidget -> setMovement(QListView::Static);
    m_optionsListWidget -> setMaximumWidth(170);
    m_optionsListWidget -> setSpacing(7);
    m_optionsListWidget -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add options
    m_optionsListWidget -> addItem(tr("General"));
    m_optionsListWidget -> item(0) -> setIcon(QIcon::fromTheme("preferences-plugin",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));

    m_optionsListWidget -> addItem(tr("Hardware"));
    m_optionsListWidget -> item(1) -> setIcon(QIcon::fromTheme("cpu",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/update-none.svg"))));

    m_optionsListWidget -> addItem(tr("Boot options"));
    m_optionsListWidget -> item(2) -> setIcon(QIcon::fromTheme("checkmark",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/applications-education-language.svg"))));

    m_optionsListWidget -> addItem(tr("Media"));
    m_optionsListWidget -> item(3) -> setIcon(QIcon::fromTheme("ntfs-config",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/practice-start.svg"))));

    m_optionsListWidget -> addItem(tr("Network"));
    m_optionsListWidget -> item(4) -> setIcon(QIcon::fromTheme("network-card",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    m_optionsListWidget -> addItem(tr("Accelerator"));
    m_optionsListWidget -> item(5) -> setIcon(QIcon::fromTheme("mathematica",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    m_optionsListWidget -> addItem(tr("Display"));
    m_optionsListWidget -> item(6) -> setIcon(QIcon::fromTheme("applications-multimedia",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    // Prepare window
    m_optionsStackedWidget = new QStackedWidget(this);
    m_optionsStackedWidget -> setSizePolicy(QSizePolicy::Preferred,
                                            QSizePolicy::MinimumExpanding);

    m_optionsStackedWidget -> addWidget(this -> m_generalPageWidget);

    connect(m_optionsListWidget, &QListWidget::currentRowChanged,
            m_optionsStackedWidget, &QStackedWidget::setCurrentIndex);

    m_topLayout = new QHBoxLayout();
    m_topLayout -> addWidget(m_optionsListWidget);
    m_topLayout -> addWidget(m_optionsStackedWidget);

    // Buttons
    m_saveButton = new QPushButton(QIcon::fromTheme("document-save",
                                                    QIcon(QPixmap(":/images/icons/breeze/32x32/document-save.svg"))),
                                   tr("Save"),
                                   this);
    connect(m_saveButton, &QAbstractButton::clicked,
            this, &MachineConfigWindow::saveMachineSettings);

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
    m_mainLayout -> addLayout(m_buttonsLayout);

    this -> setLayout(m_mainLayout);

    this -> m_optionsListWidget -> setCurrentRow(0);
    this -> m_optionsListWidget -> setFocus();

    qDebug() << "MachineConfigWindow created";

}

MachineConfigWindow::~MachineConfigWindow() {
    qDebug() << "MachineConfigWindow destroyed";
}

void MachineConfigWindow::createGeneralPage() {

    m_machineGeneralGrpBox = new QGroupBox(tr("General data"));

    m_OSType = new QComboBox();
    m_OSType -> setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::MinimumExpanding);

    m_OSType -> addItem("GNU/Linux");
    m_OSType -> addItem("Microsoft Windows");
    m_OSType -> addItem("BSD");
    m_OSType -> addItem(tr("Other"));

    connect(m_OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MachineConfigWindow::selectOS);

    m_OSVersion = new QComboBox();
    m_OSVersion -> setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);

    this -> selectOS(0);

    m_machineNameLineEdit = new QLineEdit();
    m_machineUuidLabel = new QLabel();
    m_machineStatusLabel = new QLabel();

    m_machineDescTextEdit = new QPlainTextEdit();

    m_machineDataLayout = new QFormLayout();
    m_machineDataLayout -> setAlignment(Qt::AlignLeft);
    m_machineDataLayout -> setLabelAlignment(Qt::AlignLeft);
    m_machineDataLayout -> setHorizontalSpacing(20);
    m_machineDataLayout -> setVerticalSpacing(20);
    m_machineDataLayout -> addRow(tr("Name") + ":", m_machineNameLineEdit);
    m_machineDataLayout -> addRow(tr("Type") + ":", m_OSType);
    m_machineDataLayout -> addRow(tr("Version") + ":", m_OSVersion);
    m_machineDataLayout -> addRow(tr("UUID") + ":", m_machineUuidLabel);
    m_machineDataLayout -> addRow(tr("Status") + ":", m_machineStatusLabel);
    m_machineDataLayout -> addRow(tr("Description") + ":", m_machineDescTextEdit);

    m_machineGeneralGrpBox -> setLayout(m_machineDataLayout);

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout -> addWidget(m_machineGeneralGrpBox);

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget -> setLayout(m_generalPageLayout);
}

void MachineConfigWindow::selectOS(int OSSelected){

    this -> m_OSVersion -> clear();

    if (OSSelected == 0) {
        this -> m_OSVersion -> addItem(tr("Debian"));
        this -> m_OSVersion -> addItem(tr("Ubuntu"));
        this -> m_OSVersion -> addItem(tr("Fedora"));
        this -> m_OSVersion -> addItem(tr("OpenSuse"));
        this -> m_OSVersion -> addItem(tr("Mageia"));
        this -> m_OSVersion -> addItem(tr("Gentoo"));
        this -> m_OSVersion -> addItem(tr("Arch Linux"));
        this -> m_OSVersion -> addItem(tr("Linux"));
    } else if (OSSelected == 1) {
        this -> m_OSVersion -> addItem(tr("Microsoft 95"));
        this -> m_OSVersion -> addItem(tr("Microsoft 98"));
        this -> m_OSVersion -> addItem(tr("Microsoft 2000"));
        this -> m_OSVersion -> addItem(tr("Microsoft XP"));
        this -> m_OSVersion -> addItem(tr("Microsoft Vista"));
        this -> m_OSVersion -> addItem(tr("Microsoft 7"));
        this -> m_OSVersion -> addItem(tr("Microsoft 8"));
        this -> m_OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == 2) {
        this -> m_OSVersion -> addItem(tr("FreeBSD"));
        this -> m_OSVersion -> addItem(tr("OpenBSD"));
        this -> m_OSVersion -> addItem(tr("NetBSD"));
    } else if (OSSelected == 3) {
        this -> m_OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> m_OSVersion -> addItem(tr("Arch Hurd"));
        this -> m_OSVersion -> addItem(tr("Redox"));
        this -> m_OSVersion -> addItem(tr("ReactOS"));
    }
}

void MachineConfigWindow::saveMachineSettings() {

}

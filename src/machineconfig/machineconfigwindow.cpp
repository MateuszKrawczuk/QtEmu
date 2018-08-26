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
#include "machineconfigwindow.h"

MachineConfigWindow::MachineConfigWindow(Machine *machine,
                                         QEMU *QEMUGlobalObject,
                                         QWidget *parent) : QWidget(parent) {

    this -> m_machine = machine;

    this -> setWindowTitle(tr("Machine Preferences") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/images/qtemu.png")));
    this -> setWindowFlags(Qt::Dialog);
    this -> setWindowModality(Qt::ApplicationModal);
    this -> setMinimumSize(640, 500);

    m_configGeneral  = new MachineConfigGeneral(machine, this);
    m_configHardware = new MachineConfigHardware(machine, this);
    m_configBoot     = new MachineConfigBoot(machine, this);
    m_configMedia    = new MachineConfigMedia(machine, QEMUGlobalObject, this);
    m_configNetwork  = new MachineConfigNetwork(machine, this);
    m_configAudio    = new MachineConfigAudio(machine, this);
    m_configAccel    = new MachineConfigAccel(machine, this);

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

    m_optionsListWidget -> addItem(tr("Audio"));
    m_optionsListWidget -> item(5) -> setIcon(QIcon::fromTheme("audio-card",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));// TODO

    m_optionsListWidget -> addItem(tr("Accelerator"));
    m_optionsListWidget -> item(6) -> setIcon(QIcon::fromTheme("mathematica",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    /*m_optionsListWidget -> addItem(tr("Display"));
    m_optionsListWidget -> item(7) -> setIcon(QIcon::fromTheme("applications-multimedia",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));*/

    // Prepare window
    m_optionsStackedWidget = new QStackedWidget(this);
    m_optionsStackedWidget -> setSizePolicy(QSizePolicy::Expanding,
                                            QSizePolicy::Expanding);
    m_optionsStackedWidget -> addWidget(this -> m_configGeneral  -> m_generalPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_configHardware -> m_hardwarePageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_configBoot     -> m_bootPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_configMedia    -> m_mediaPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_configNetwork  -> m_networkPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_configAudio    -> m_audioPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_configAccel    -> m_acceleratorPageWidget);

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

/**
 * @brief Save the machine configuration
 *
 * Save the machine configuration
 */
void MachineConfigWindow::saveMachineSettings() {
    qDebug() << "Machine settings saved";

    this -> m_configGeneral -> saveGeneralData();
}

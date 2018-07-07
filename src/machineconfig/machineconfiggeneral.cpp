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
#include "machineconfiggeneral.h"

/**
 * @brief Configuration of the machine. General page
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * Configuration of the machine. General page
 * with basic data, name, os version, os type,
 * state of the machine...
 */
MachineConfigGeneral::MachineConfigGeneral(Machine *machine,
                                           QWidget *parent) : QWidget(parent) {

    bool enableFields = true;

    if (machine -> getState() != Machine::Stopped) {
        enableFields = false;
    }

    m_generalTabWidget = new QTabWidget();
    m_generalTabWidget -> setSizePolicy(QSizePolicy::MinimumExpanding,
                                            QSizePolicy::MinimumExpanding);
    m_generalTabWidget -> addTab(new BasicTab(machine, enableFields, this), tr("Basic Details"));
    m_generalTabWidget -> addTab(new DescriptionTab(machine, enableFields, this), tr("Description"));

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout -> setAlignment(Qt::AlignCenter);
    m_generalPageLayout -> addWidget(m_generalTabWidget);

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_generalPageWidget -> setLayout(m_generalPageLayout);

    qDebug() << "MachineConfigGeneral created";
}

MachineConfigGeneral::~MachineConfigGeneral() {
    qDebug() << "MachineConfigGeneral destroyed";
}

void MachineConfigGeneral::saveGeneralData() {
    qDebug() << "Save General Data";
}

BasicTab::BasicTab(Machine *machine,
                   bool enableFields,
                   QWidget *parent) : QWidget(parent) {

    m_machineNameLineEdit = new QLineEdit();
    m_machineNameLineEdit -> setText(machine -> getName());
    m_machineNameLineEdit -> setEnabled(enableFields);

    m_OSType = new QComboBox();
    m_OSType -> setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::MinimumExpanding);
    m_OSType -> setEnabled(enableFields);

    m_OSType -> addItem("GNU/Linux");
    m_OSType -> addItem("Microsoft Windows");
    m_OSType -> addItem("BSD");
    m_OSType -> addItem(tr("Other"));
    m_OSType -> setCurrentText(machine -> getOSType());

    connect(m_OSType, &QComboBox::currentTextChanged,
           this, &BasicTab::selectOS);

    m_OSVersion = new QComboBox();
    m_OSVersion -> setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);
    m_OSVersion -> setEnabled(enableFields);
    this -> selectOS(machine -> getOSType());
    m_OSVersion -> setCurrentText(machine -> getOSVersion());

    m_machineUuidLabel = new QLabel();
    m_machineUuidLabel -> setText(machine -> getUuid());
    m_machineStatusLabel = new QLabel();
    m_machineStatusLabel -> setText(BasicTab::getStatusLabel(machine -> getState()));

    m_basicTabFormLayout = new QFormLayout();
    m_basicTabFormLayout -> setAlignment(Qt::AlignTop);
    m_basicTabFormLayout -> setLabelAlignment(Qt::AlignLeft);
    m_basicTabFormLayout -> setHorizontalSpacing(20);
    m_basicTabFormLayout -> setVerticalSpacing(10);
    m_basicTabFormLayout -> addRow(tr("Name") + ":", m_machineNameLineEdit);
    m_basicTabFormLayout -> addRow(tr("Type") + ":", m_OSType);
    m_basicTabFormLayout -> addRow(tr("Version") + ":", m_OSVersion);
    m_basicTabFormLayout -> addRow(tr("UUID") + ":", m_machineUuidLabel);
    m_basicTabFormLayout -> addRow(tr("Status") + ":", m_machineStatusLabel);

    m_basicTabLayout = new QVBoxLayout();
    m_basicTabLayout -> addItem(m_basicTabFormLayout);

    this -> setLayout(m_basicTabLayout);

    qDebug() << "BasicTab created";
}

BasicTab::~BasicTab() {
    qDebug() << "BasicTab destroyed";
}

void BasicTab::selectOS(QString OSSelected){

    this -> m_OSVersion -> clear();

    if (OSSelected == "GNU/Linux") {
        this -> m_OSVersion -> addItem(tr("Debian"));
        this -> m_OSVersion -> addItem(tr("Ubuntu"));
        this -> m_OSVersion -> addItem(tr("Fedora"));
        this -> m_OSVersion -> addItem(tr("OpenSuse"));
        this -> m_OSVersion -> addItem(tr("Mageia"));
        this -> m_OSVersion -> addItem(tr("Gentoo"));
        this -> m_OSVersion -> addItem(tr("Arch Linux"));
        this -> m_OSVersion -> addItem(tr("Linux"));
    } else if (OSSelected == "Microsoft Windows") {
        this -> m_OSVersion -> addItem(tr("Microsoft 95"));
        this -> m_OSVersion -> addItem(tr("Microsoft 98"));
        this -> m_OSVersion -> addItem(tr("Microsoft 2000"));
        this -> m_OSVersion -> addItem(tr("Microsoft XP"));
        this -> m_OSVersion -> addItem(tr("Microsoft Vista"));
        this -> m_OSVersion -> addItem(tr("Microsoft 7"));
        this -> m_OSVersion -> addItem(tr("Microsoft 8"));
        this -> m_OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == "BSD") {
        this -> m_OSVersion -> addItem(tr("FreeBSD"));
        this -> m_OSVersion -> addItem(tr("OpenBSD"));
        this -> m_OSVersion -> addItem(tr("NetBSD"));
    } else {
        this -> m_OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> m_OSVersion -> addItem(tr("Arch Hurd"));
        this -> m_OSVersion -> addItem(tr("Redox"));
        this -> m_OSVersion -> addItem(tr("ReactOS"));
    }
}

QString BasicTab::getStatusLabel(Machine::States state) {

    QString statusLabel;

    switch (state) {
        case Machine::Started:
            statusLabel = tr("Started");
            break;
        case Machine::Stopped:
            statusLabel = tr("Stopped");
            break;
        case Machine::Saved:
            statusLabel = tr("Saved");
            break;
        case Machine::Paused:
            statusLabel = tr("Paused");
            break;
        default:
            statusLabel = tr("Stopped");
            break;
    }

    return statusLabel;
}

DescriptionTab::DescriptionTab(Machine *machine,
                               bool enableFields,
                               QWidget *parent) : QWidget(parent) {

    m_machineDescLabel = new QLabel(tr("Description") + ":");
    m_machineDescTextEdit = new QPlainTextEdit();
    m_machineDescTextEdit -> setEnabled(enableFields);
    m_machineDescTextEdit -> setPlainText(machine -> getDescription());

    m_descriptionLayout = new QVBoxLayout();
    m_descriptionLayout -> setAlignment(Qt::AlignVCenter);
    m_descriptionLayout -> addWidget(m_machineDescLabel);
    m_descriptionLayout -> addWidget(m_machineDescTextEdit);

    this -> setLayout(m_descriptionLayout);

    qDebug() << "DescriptionTab created";
}

DescriptionTab::~DescriptionTab() {
    qDebug() << "DescriptionTab destroyed";
}

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
#include "generalpage.h"

MachineNamePage::MachineNamePage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    this -> setTitle(tr("Machine name and operating system"));

    this -> m_newMachine = machine;

    m_descriptionNameLabel = new QLabel(tr("Select name and operating system for your new machine."));
    m_descriptionNameLabel -> setWordWrap(true);

    m_machineNameLabel = new QLabel(tr("Name") + ":");
    m_machineNameLineEdit = new QLineEdit();
    m_machineNameLineEdit -> setMaxLength(250);

    m_OSTypeLabel = new QLabel(tr("Type") + ":");
    m_OSType = new QComboBox();

    m_OSType -> addItem("GNU/Linux");
    m_OSType -> addItem("Microsoft Windows");
    m_OSType -> addItem("BSD");
    m_OSType -> addItem(tr("Other"));

    connect(m_OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MachineNamePage::selectOS);

    m_OSVersionLabel = new QLabel(tr("Version") + ":");
    m_OSVersion = new QComboBox();

    this -> selectOS(0);

    this -> registerField("machine.name*", m_machineNameLineEdit);
    this -> registerField("machine.ostype", m_OSType, "currentText", "currentTextChanged");
    this -> registerField("machine.osversion", m_OSVersion, "currentText", "currentTextChanged");

    this -> m_machineFolder = QString();

    m_mainLayout = new QGridLayout();

    m_mainLayout -> setColumnStretch(1, 10);
    m_mainLayout -> setColumnStretch(2, 10);
    m_mainLayout -> setColumnStretch(3, 10);

    m_mainLayout -> addWidget(m_descriptionNameLabel, 0, 0, 1, 4);
    m_mainLayout -> addWidget(m_machineNameLabel,     1, 0, 1, 1);
    m_mainLayout -> addWidget(m_machineNameLineEdit,  1, 1, 1, 3);
    m_mainLayout -> addWidget(m_OSTypeLabel,          2, 0, 1, 1);
    m_mainLayout -> addWidget(m_OSType,               2, 1, 1, 3);
    m_mainLayout -> addWidget(m_OSVersionLabel,       3, 0, 1, 1);
    m_mainLayout -> addWidget(m_OSVersion,            3, 1, 1, 3);

    this -> setLayout(m_mainLayout);

    qDebug() << "MachineNamePage created";
}

MachineNamePage::~MachineNamePage() {
    qDebug() << "MachineNamePage destroyed";
}

void MachineNamePage::selectOS(int OSSelected){

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

bool MachineNamePage::validatePage() {

    if ( ! this -> m_machineFolder.isEmpty() ) {
        return true;
    }

    QSettings settings;
    settings.beginGroup("Configuration");

    QString strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

    settings.endGroup();

    QString strMachineName = field("machine.name").toString();
    QString strMachinePathMsg = strMachinePath;
    QString strFullMachinePath = strMachinePath.append("/").append(strMachineName);
    QString strMachineLogsPath = strMachinePath.append("/").append("logs");

    if ( QDir(strFullMachinePath).exists() ) {
        qDebug() << "The folder alredy exists" << strFullMachinePath;

        m_createMachineMessageBox = new QMessageBox();
        m_createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        m_createMachineMessageBox -> setIcon(QMessageBox::Critical);
        m_createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                                "in the parent folder <strong>%2<strong></p>"
                                                "<p>This folder alredy exists and possibly belongs to another machine.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        m_createMachineMessageBox -> exec();

        return false;
    }

    if ( ! QDir().mkpath(strFullMachinePath) || ! QDir().mkpath(strMachineLogsPath)) {
        qDebug() << "Machine folder not created" << strFullMachinePath;

        m_createMachineMessageBox = new QMessageBox();
        m_createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        m_createMachineMessageBox -> setIcon(QMessageBox::Critical);
        m_createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                                "in the parent folder <strong>%2<strong></p>"
                                                "<p>Please check that the parent really exists and that "
                                                "you have permissions to create the machine folder.</p>")
                                             .arg(strMachineName).arg(strMachinePathMsg));
        m_createMachineMessageBox -> exec();

        return false;
    }

    this -> m_machineFolder = strFullMachinePath;

    // Set all the values in the machine object
    this -> m_newMachine -> setName(this -> m_machineNameLineEdit -> text());
    this -> m_newMachine -> setOSType(this -> m_OSType -> currentText());
    this -> m_newMachine -> setOSVersion(this -> m_OSVersion -> currentText());
    this -> m_newMachine -> setPath(strFullMachinePath);

    return true;
}

void MachineNamePage::cleanupPage() {
    if ( ! this -> m_machineFolder.isEmpty() ) {
        QDir machineDir(this -> m_machineFolder);
        machineDir.removeRecursively();
    }
}

void MachineNamePage::initializePage() {
    this -> m_machineFolder = QString();
}

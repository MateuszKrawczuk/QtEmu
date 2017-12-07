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
#include "generalpage.h"

MachineNamePage::MachineNamePage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    this -> setTitle(tr("Machine name and operating system"));

    this -> newMachine = machine;

    descriptionNameLabel = new QLabel(tr("Select name and operating system for your new machine."));
    descriptionNameLabel -> setWordWrap(true);

    machineNameLabel = new QLabel(tr("Name") + ":");
    machineNameLineEdit = new QLineEdit();

    OSTypeLabel = new QLabel(tr("Type") + ":");
    OSType = new QComboBox();

    OSType -> addItem("GNU/Linux");
    OSType -> addItem("Microsoft Windows");
    OSType -> addItem("BSD");
    OSType -> addItem(tr("Other"));

    connect(OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MachineNamePage::selectOS);

    OSVersionLabel = new QLabel(tr("Version") + ":");
    OSVersion = new QComboBox();

    this -> selectOS(0);

    this -> registerField("machine.name*", machineNameLineEdit);
    this -> registerField("machine.ostype", OSType, "currentText", "currentTextChanged");
    this -> registerField("machine.osversion", OSVersion, "currentText", "currentTextChanged");

    machineFolderCreated = QString();

    mainLayout = new QGridLayout();

    mainLayout -> setColumnStretch(1, 10);
    mainLayout -> setColumnStretch(2, 10);
    mainLayout -> setColumnStretch(3, 10);

    mainLayout -> addWidget(descriptionNameLabel, 0, 0, 1, 4);
    mainLayout -> addWidget(machineNameLabel,     1, 0, 1, 1);
    mainLayout -> addWidget(machineNameLineEdit,  1, 1, 1, 3);
    mainLayout -> addWidget(OSTypeLabel,          2, 0, 1, 1);
    mainLayout -> addWidget(OSType,               2, 1, 1, 3);
    mainLayout -> addWidget(OSVersionLabel,       3, 0, 1, 1);
    mainLayout -> addWidget(OSVersion,            3, 1, 1, 3);

    this ->  setLayout(mainLayout);

    qDebug() << "MachineNamePage created";
}

MachineNamePage::~MachineNamePage() {
    qDebug() << "MachineNamePage destroyed";
}

void MachineNamePage::selectOS(int OSSelected){

    this -> OSVersion -> clear();

    if (OSSelected == 0) {
        this -> OSVersion -> addItem(tr("Debian"));
        this -> OSVersion -> addItem(tr("Ubuntu"));
        this -> OSVersion -> addItem(tr("Fedora"));
        this -> OSVersion -> addItem(tr("OpenSuse"));
        this -> OSVersion -> addItem(tr("Mageia"));
        this -> OSVersion -> addItem(tr("Gentoo"));
        this -> OSVersion -> addItem(tr("Arch Linux"));
        this -> OSVersion -> addItem(tr("Linux 4.x"));
    } else if (OSSelected == 1) {
        this -> OSVersion -> addItem(tr("Microsoft 95"));
        this -> OSVersion -> addItem(tr("Microsoft 98"));
        this -> OSVersion -> addItem(tr("Microsoft 2000"));
        this -> OSVersion -> addItem(tr("Microsoft XP"));
        this -> OSVersion -> addItem(tr("Microsoft Vista"));
        this -> OSVersion -> addItem(tr("Microsoft 7"));
        this -> OSVersion -> addItem(tr("Microsoft 8"));
        this -> OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == 2) {
        this -> OSVersion -> addItem(tr("FreeBSD"));
        this -> OSVersion -> addItem(tr("OpenBSD"));
        this -> OSVersion -> addItem(tr("NetBSD"));
    } else if (OSSelected == 3) {
        this -> OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> OSVersion -> addItem(tr("Arch Hurd"));
        this -> OSVersion -> addItem(tr("Minix"));
        this -> OSVersion -> addItem(tr("ReactOS"));
    }
}

bool MachineNamePage::validatePage() {

    if ( ! machineFolderCreated.isEmpty() ) {
        return true;
    }

    QSettings settings;
    settings.beginGroup("Configuration");

    QString strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

    settings.endGroup();

    QString strMachineName = field("machine.name").toString();
    QString strMachinePathMsg = strMachinePath;
    QString strFullMachinePath = strMachinePath.append("/").append(strMachineName);

    if ( QDir(strFullMachinePath).exists() ) {
        qDebug() << "The folder alredy exists" << strFullMachinePath;

        createMachineMessageBox = new QMessageBox();
        createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        createMachineMessageBox -> setIcon(QMessageBox::Critical);
        createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                              "in the parent folder <strong>%2<strong></p>"
                                              "<p>This folder alredy exists and possibly belongs to another machine.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        createMachineMessageBox -> exec();

        return false;
    }

    if ( ! QDir().mkpath(strFullMachinePath) ) {
        qDebug() << "Machine folder created" << strFullMachinePath;

        createMachineMessageBox = new QMessageBox();
        createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        createMachineMessageBox -> setIcon(QMessageBox::Critical);
        createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                              "in the parent folder <strong>%2<strong></p>"
                                              "<p>Please check that the parent really exists and that "
                                              "you have permissions to create the machine folder.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        createMachineMessageBox->exec();

        return false;
    }

    machineFolderCreated = strFullMachinePath;

    // Set all the values in the machine object
    this -> newMachine -> setName(this -> machineNameLineEdit -> text());
    this -> newMachine -> setOSType(this -> OSType -> currentText());
    this -> newMachine -> setOSVersion(this -> OSVersion -> currentText());

    return true;
}

void MachineNamePage::cleanupPage() {

    if ( ! machineFolderCreated.isEmpty() ) {
        QDir().rmpath(machineFolderCreated);
    }

}

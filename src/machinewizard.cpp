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
#include "machinewizard.h"

MachineWizard::MachineWizard(QWidget *parent) : QWizard(parent) {

    setPage(Page_Name, new MachineNamePage(this));
    setPage(Page_Memory, new MachineMemoryPage(this));
    setPage(Page_Disk, new MachineDiskPage(this));
    setPage(Page_Hardware, new MachineHardwarePage(this));
    setPage(Page_Conclusion, new MachineConclusionPage(this));

    setStartId(Page_Name);

    #ifndef Q_OS_MAC
        setWizardStyle(ClassicStyle);
    #endif

    #ifdef Q_OS_MAC
        setWizardStyle(MacStyle);
    #endif

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    setWindowTitle(tr("Create a new Machine"));

    qDebug() << "MachineWizard created";
}

MachineWizard::~MachineWizard() {
    qDebug() << "MachineWizard destroyed";
}

MachineNamePage::MachineNamePage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine name and operating system"));

    descriptionLabel = new QLabel(tr("Select name and operating system for your new machine."));
    machineNameLabel = new QLabel(tr("Name") + ":");
    OSLabel = new QLabel(tr("Operating system") + ":");
    OSTypeLabel = new QLabel(tr("Version:") + ":");

    machineNameLineEdit = new QLineEdit();

    machineNameLayout = new QHBoxLayout();
    machineNameLayout -> setAlignment(Qt::AlignVCenter);
    machineNameLayout -> setSpacing(5);

    machineNameLayout -> addWidget(machineNameLabel);
    machineNameLayout -> addWidget(machineNameLineEdit);

    machineLayout = new QVBoxLayout;
    machineLayout -> addWidget(descriptionLabel);
    machineLayout -> addItem(machineNameLayout);
    setLayout(machineLayout);
}

MachineNamePage::~MachineNamePage() {
    qDebug() << "MachineNamePage destroyed";
}

MachineMemoryPage::MachineMemoryPage(QWidget *parent) : QWizardPage(parent) {

}

MachineMemoryPage::~MachineMemoryPage() {
    qDebug() << "MachineMemoryPage destroyed";
}

MachineDiskPage::MachineDiskPage(QWidget *parent) : QWizardPage(parent) {

}

MachineDiskPage::~MachineDiskPage() {
    qDebug() << "MachineDiskPage destroyed";
}

MachineHardwarePage::MachineHardwarePage(QWidget *parent) : QWizardPage(parent) {

}

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
}

MachineConclusionPage::MachineConclusionPage(QWidget *parent) : QWizardPage(parent) {

}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

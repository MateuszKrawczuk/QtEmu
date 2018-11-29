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
#include "machinewizard.h"

/**
 * @brief New machine wizard
 * @param machine, new machine object
 * @param osListWidget, list with all the machines from the mainwindow
 * @param QEMUGlobalObject, QEMU global object with data about QEMU
 * @param parent, parent widget
 *
 * New machine wizard, following this wizard the user can create a
 * complete machine
 */
MachineWizard::MachineWizard(Machine *machine,
                             QListWidget *osListWidget,
                             QEMU *QEMUGlobalObject,
                             QWidget *parent) : QWizard(parent)
{
    this->setWindowTitle(tr("Create a new Machine"));

    this->setPage(Page_Name, new MachineNamePage(machine, this));
    this->setPage(Page_Hardware, new MachineHardwarePage(machine, this));
    this->setPage(Page_Accelerator, new MachineAcceleratorPage(machine, this));
    this->setPage(Page_Memory, new MachineMemoryPage(machine, this));
    this->setPage(Page_Disk, new MachineDiskPage(machine, this));
    this->setPage(Page_New_Disk, new MachineNewDiskPage(machine ,this));
    this->setPage(Page_Conclusion, new MachineConclusionPage(machine, osListWidget, QEMUGlobalObject, this));

    this->setStartId(Page_Name);

    #ifndef Q_OS_MAC
        this->setWizardStyle(ClassicStyle);
    #endif
    #ifdef Q_OS_MAC
        this->setWizardStyle(MacStyle);
    #endif

    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    this->setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    this->setMinimumSize(700, 400);

    qDebug() << "MachineWizard created";
}

MachineWizard::~MachineWizard()
{
    qDebug() << "MachineWizard destroyed";
}

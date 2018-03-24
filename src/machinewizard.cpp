/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
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
#include "machinewizard.h"

MachineWizard::MachineWizard(Machine *machine, QListWidget *osListWidget,
                             QWidget *parent) : QWizard(parent) {

    setWindowTitle(tr("Create a new Machine"));

    setPage(Page_Name, new MachineNamePage(machine, this));
    setPage(Page_Hardware, new MachineHardwarePage(machine, this));
    setPage(Page_Accelerator, new MachineAcceleratorPage(machine, this));
    setPage(Page_Memory, new MachineMemoryPage(machine, this));
    setPage(Page_Disk, new MachineDiskPage(machine, this));
    setPage(Page_New_Disk, new MachineNewDiskPage(machine ,this));
    setPage(Page_Conclusion, new MachineConclusionPage(machine, osListWidget, this));

    setStartId(Page_Name);

    #ifndef Q_OS_MAC
        setWizardStyle(ClassicStyle);
    #endif

    #ifdef Q_OS_MAC
        setWizardStyle(MacStyle);
    #endif

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    setMinimumSize(700, 400);

    qDebug() << "MachineWizard created";
}

MachineWizard::~MachineWizard() {
    qDebug() << "MachineWizard destroyed";
}

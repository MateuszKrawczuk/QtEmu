// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
    this->setPage(Page_Machine, new MachinePage(machine, this));
    this->setPage(Page_Hardware, new MachineHardwarePage(machine, QEMUGlobalObject, this));
    this->setPage(Page_Accelerator, new MachineAcceleratorPage(machine, QEMUGlobalObject, this));
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

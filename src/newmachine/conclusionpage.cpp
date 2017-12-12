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
#include "conclusionpage.h"

MachineConclusionPage::MachineConclusionPage(Machine *machine,
                                             QWidget *parent) : QWizardPage(parent) {

    this -> setTitle(tr("Machine Summary"));

    this -> newMachine = machine;

    conclusionLabel = new QLabel(tr("Summary of the new machine"));

    machineDescLabel = new QLabel(tr("Name") + ":");
    OSTypeDescLabel = new QLabel(tr("Operating system") + ":");
    OSVersionDescLabel = new QLabel(tr("Operating system version") + ":");
    processorDescLabel = new QLabel(tr("Processor") + ":");
    graphicsDescLabel = new QLabel(tr("Graphics") + ":");
    audioDescLabel = new QLabel(tr("Audio") + ":");
    RAMDescLabel = new QLabel(tr("RAM") + ":");
    acceleratorDescLabel = new QLabel(tr("Accelerator") + ":");
    diskDescLabel = new QLabel(tr("Disk") + ":");

    machineNameLabel = new QLabel();
    machineNameLabel -> setWordWrap(true);
    OSTypeLabel      = new QLabel();
    OSVersionLabel   = new QLabel();
    processorLabel   = new QLabel();
    graphicsLabel    = new QLabel();
    audioLabel       = new QLabel();
    audioLabel -> setWordWrap(true);
    RAMLabel         = new QLabel();
    acceleratorLabel = new QLabel();
    diskLabel        = new QLabel();
    diskLabel -> setWordWrap(true);

    conclusionLayout = new QGridLayout();

    conclusionLayout -> setColumnStretch(1, 10);
    conclusionLayout -> setColumnStretch(2, 10);
    conclusionLayout -> setColumnStretch(3, 10);

    conclusionLayout -> addWidget(conclusionLabel,      0, 0, 1, 4);
    conclusionLayout -> addWidget(machineDescLabel,     1, 0, 1, 1);
    conclusionLayout -> addWidget(machineNameLabel,     1, 1, 1, 1);
    conclusionLayout -> addWidget(OSTypeDescLabel,      2, 0, 1, 1);
    conclusionLayout -> addWidget(OSTypeLabel,          2, 1, 1, 1);
    conclusionLayout -> addWidget(OSVersionDescLabel,   3, 0, 1, 1);
    conclusionLayout -> addWidget(OSVersionLabel,       3, 1, 1, 1);
    conclusionLayout -> addWidget(processorDescLabel,   4, 0, 1, 1);
    conclusionLayout -> addWidget(processorLabel,       4, 1, 1, 1);
    conclusionLayout -> addWidget(graphicsDescLabel,    5, 0, 1, 1);
    conclusionLayout -> addWidget(graphicsLabel,        5, 1, 1, 1);
    conclusionLayout -> addWidget(audioDescLabel,       6, 0, 1, 1);
    conclusionLayout -> addWidget(audioLabel,           6, 1, 1, 3);
    conclusionLayout -> addWidget(RAMDescLabel,         7, 0, 1, 1);
    conclusionLayout -> addWidget(RAMLabel,             7, 1, 1, 1);
    conclusionLayout -> addWidget(acceleratorDescLabel, 8, 0, 1, 1);
    conclusionLayout -> addWidget(acceleratorLabel,     8, 1, 1, 1);
    conclusionLayout -> addWidget(diskDescLabel,        9, 0, 1, 1);
    conclusionLayout -> addWidget(diskLabel,            9, 1, 1, 1);

    this -> setLayout(conclusionLayout);

    qDebug() << "MachineConclusionPage created";
}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

void MachineConclusionPage::initializePage() {
    this -> machineNameLabel -> setText(this -> newMachine -> getName());
    this -> OSTypeLabel      -> setText(this -> newMachine -> getOSType());
    this -> OSVersionLabel   -> setText(this -> newMachine -> getOSVersion());
    this -> processorLabel   -> setText(this -> newMachine -> getCPUType());
    this -> graphicsLabel    -> setText(this -> newMachine -> getGPUType());
    this -> RAMLabel         -> setText(QString::number(this -> newMachine -> getRAM()).append(" MiB"));
    this -> audioLabel       -> setText(this -> newMachine -> getAudioLabel());
    this -> acceleratorLabel -> setText(this -> newMachine -> getAcceleratorLabel());
    this -> diskLabel        -> setText(this -> newMachine -> getDiskPath());

}

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

    conclusionLayout = new QFormLayout();
    conclusionLayout -> addRow(tr("Name") + ":", new QLabel(this -> newMachine -> getName()));
    conclusionLayout -> addRow(tr("Processor") + ":", new QLabel(this -> newMachine -> getCPUType()));
    conclusionLayout -> addRow(tr("Graphics") + ":", new QLabel(this -> newMachine -> getGPUType()));
    //conclusionLayout -> addRow(tr("Audio") + ":", new QLabel(this -> newMachine -> getAudio()));
    conclusionLayout -> addRow(tr("RAM") + ":", new QLabel(QString::number(this -> newMachine -> getRAM())));
    //conclusionLayout -> addRow(tr("Accelerator") + ":", new QLabel(this -> newMachine -> getAccelerator()));
    //conclusionLayout -> addRow(tr("Disk") + ":", );

    this -> setLayout(conclusionLayout);

    qDebug() << "MachineConclusionPage created";
}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

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
#include "memorypage.h"

MachineMemoryPage::MachineMemoryPage(Machine *machine,
                                     QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine memory"));

    this -> newMachine = machine;

    descriptionMemoryLabel = new QLabel(
                tr("Select the amount of base memory (RAM) in megabytes for virtual machine allocating."));
    descriptionMemoryLabel -> setWordWrap(true);

    int32_t totalRAM = 0;

    SystemUtils::getTotalMemory(totalRAM);

    spinBoxMemoryLabel = new QLabel("MiB");

    memorySpinBox = new QSpinBox();
    memorySpinBox -> setMinimum(1);
    memorySpinBox -> setMaximum(totalRAM);

    this -> registerField("machine.ram", memorySpinBox, "value", "valueChanged");

    memorySlider = new QSlider(Qt::Horizontal);
    memorySlider -> setTickPosition(QSlider::TicksBelow);
    memorySlider -> setTickInterval(500);
    memorySlider -> setMinimum(1);
    memorySlider -> setMaximum(totalRAM);

    connect(memorySpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            memorySlider, &QSlider::setValue);

    connect(memorySlider, &QSlider::valueChanged,
            memorySpinBox, &QSpinBox::setValue);

    minMemoryLabel = new QLabel("1 MiB");
    maxMemorylabel = new QLabel(QString("%1 MiB").arg(totalRAM));

    machineMemoryLayout = new QGridLayout();

    machineMemoryLayout -> setColumnStretch(1, 50);

    machineMemoryLayout -> addWidget(descriptionMemoryLabel, 0, 0, 1, 5);
    machineMemoryLayout -> addWidget(memorySlider,           1, 0, 1, 3);
    machineMemoryLayout -> addWidget(memorySpinBox,          1, 3, 1, 1);
    machineMemoryLayout -> addWidget(spinBoxMemoryLabel,     1, 4, 1, 1);
    machineMemoryLayout -> addWidget(minMemoryLabel,         2, 0, 1, 1);
    machineMemoryLayout -> addWidget(maxMemorylabel,         2, 2, 1, 1);

    setLayout(machineMemoryLayout);

    qDebug() << "MachineMemoryPage created";
}

MachineMemoryPage::~MachineMemoryPage() {
    qDebug() << "MachineMemoryPage destroyed";
}

bool MachineMemoryPage::validatePage() {
    this -> newMachine -> setRAM(this -> memorySpinBox -> value());

    return true;
}

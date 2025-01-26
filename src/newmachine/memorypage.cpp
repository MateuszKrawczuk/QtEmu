/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include "memorypage.h"

/**
 * @brief Memory page
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Memory page section. In this page you can select the amount of memory for
 * the new machine
 */
MachineMemoryPage::MachineMemoryPage(Machine *machine,
                                     QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine memory"));
    this->m_newMachine = machine;

    m_descriptionMemoryLabel = new QLabel(
                tr("Select the amount of base memory (RAM) in megabytes for virtual machine allocating."), this);
    m_descriptionMemoryLabel->setWordWrap(true);

    int totalRAM = 0;
    SystemUtils::getTotalMemory(totalRAM);
    int defaultRAM = totalRAM / 4;
    m_spinBoxMemoryLabel = new QLabel("MiB", this);

    m_memorySpinBox = new QSpinBox(this);
    m_memorySpinBox->setMinimum(1);
    m_memorySpinBox->setMaximum(totalRAM);

    m_memorySpinBox->setValue(defaultRAM);
    this->registerField("machine.ram", m_memorySpinBox, "value", "valueChanged");

    m_memorySlider = new QSlider(Qt::Horizontal, this);
    m_memorySlider->setTickPosition(QSlider::TicksBelow);
    m_memorySlider->setTickInterval(totalRAM / 8); // Set tick interval to 1/8 of total RAM
    m_memorySlider->setSingleStep(128); // Set slider step to 128 MiB
    m_memorySlider->setMinimum(1);
    m_memorySlider->setMaximum(totalRAM);

    m_memorySlider->setValue(defaultRAM);

    connect(m_memorySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_memorySlider, &QSlider::setValue);

    connect(m_memorySlider, &QSlider::valueChanged,
            m_memorySpinBox, &QSpinBox::setValue);

    m_minMemoryLabel = new QLabel("1 MiB", this);
    m_maxMemorylabel = new QLabel(QString("%1 MiB").arg(totalRAM), this);

    m_machineMemoryLayout = new QGridLayout();
    m_machineMemoryLayout->setColumnStretch(1, 50);
    m_machineMemoryLayout->addWidget(m_descriptionMemoryLabel, 0, 0, 1, 5);
    m_machineMemoryLayout->addWidget(m_memorySlider,           1, 0, 1, 3);
    m_machineMemoryLayout->addWidget(m_memorySpinBox,          1, 3, 1, 1);
    m_machineMemoryLayout->addWidget(m_spinBoxMemoryLabel,     1, 4, 1, 1);
    m_machineMemoryLayout->addWidget(m_minMemoryLabel,         2, 0, 1, 1);
    m_machineMemoryLayout->addWidget(m_maxMemorylabel,         2, 2, 1, 1);

    this->setLayout(m_machineMemoryLayout);

    qDebug() << "MachineMemoryPage created";
}

MachineMemoryPage::~MachineMemoryPage()
{
    qDebug() << "MachineMemoryPage destroyed";
}

bool MachineMemoryPage::validatePage()
{
    this->m_newMachine->setRAM(this->m_memorySpinBox->value());
    return true;
}

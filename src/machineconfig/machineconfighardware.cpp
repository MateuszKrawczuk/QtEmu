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
#include "machineconfighardware.h"

/**
 * @brief Hardware configuration window
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * In this window the user can configure the hardware data
 */
MachineConfigHardware::MachineConfigHardware(Machine *machine,
                                             QWidget *parent) : QWidget(parent)
{
    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    this->m_machine = machine;

    m_hardwareTabWidget = new QTabWidget(this);
    m_hardwareTabWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                                       QSizePolicy::MinimumExpanding);

    m_processorConfigTab = new ProcessorConfigTab(machine, enableFields, this);
    m_graphicsConfigTab = new GraphicsConfigTab(machine, enableFields, this);
    m_ramConfigTab = new RamConfigTab(machine, enableFields, this);
    m_machineTypeTab = new MachineTypeTab(machine, enableFields, this);

    m_hardwareTabWidget->addTab(this->m_processorConfigTab, tr("CPU"));
    m_hardwareTabWidget->addTab(this->m_graphicsConfigTab, tr("Graphics"));
    m_hardwareTabWidget->addTab(this->m_ramConfigTab, tr("RAM"));
    m_hardwareTabWidget->addTab(this->m_machineTypeTab, tr("Type"));

    m_hardwarePageLayout = new QVBoxLayout();
    m_hardwarePageLayout->setAlignment(Qt::AlignCenter);
    m_hardwarePageLayout->addWidget(m_hardwareTabWidget);

    m_hardwarePageWidget = new QWidget(this);
    m_hardwarePageWidget->setLayout(m_hardwarePageLayout);

    qDebug() << "MachineConfigHardware created";
}

MachineConfigHardware::~MachineConfigHardware()
{
    qDebug() << "MachineConfigHardware destroyed";
}

/**
 * @brief Save the hardware data of the machine
 *
 * Save the hardware data of the machine
 */
void MachineConfigHardware::saveHardwareData()
{
    this->m_machine->setType(this->m_machineTypeTab->getMachineType());
    this->m_machine->setCPUType(this->m_processorConfigTab->getCPUType());
    this->m_machine->setCPUCount(this->m_processorConfigTab->getCPUCount());
    this->m_machine->setGPUType(this->m_graphicsConfigTab->getGPUType());
    this->m_machine->setKeyboard(this->m_graphicsConfigTab->getKeyboardLayout());
    this->m_machine->setRAM(this->m_ramConfigTab->getAmountRam());
}

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
#include "machineconfignetwork.h"

/**
 * @brief Network configuration window
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * In this window the user can enable or disable the network of the machine
 */
MachineConfigNetwork::MachineConfigNetwork(Machine *machine,
                                           QWidget *parent) : QWidget(parent)
{
    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    this->m_machine = machine;

    m_withNetworkRadio = new QRadioButton(tr("User Mode Network Connection (Uses the user mode network stack)"), this);
    m_withNetworkRadio->setEnabled(enableFields);
    if (machine->getUseNetwork() == true) {
        m_withNetworkRadio->setChecked(true);
    }

    m_withoutNetworkRadio = new QRadioButton(tr("No network (No network cards installed on this machine"), this);
    m_withoutNetworkRadio->setEnabled(enableFields);
    if (machine->getUseNetwork() == false) {
        m_withoutNetworkRadio->setChecked(true);
    }

    m_networkLayout = new QVBoxLayout();
    m_networkLayout->setAlignment(Qt::AlignTop);
    m_networkLayout->setContentsMargins(5, 20, 5, 0);
    m_networkLayout->addWidget(m_withNetworkRadio);
    m_networkLayout->addWidget(m_withoutNetworkRadio);

    m_machineNetworkGroup = new QGroupBox(tr("Machine Network"));
    m_machineNetworkGroup->setLayout(m_networkLayout);

    m_networkMainLayout = new QVBoxLayout();
    m_networkMainLayout->addWidget(m_machineNetworkGroup);

    m_networkPageWidget = new QWidget();
    m_networkPageWidget->setLayout(m_networkMainLayout);

    qDebug() << "MachineConfigNetwork created";
}

MachineConfigNetwork::~MachineConfigNetwork()
{
    qDebug() << "MachineConfigNetwork destroyed";
}

/**
 * @brief Enable or disable the network
 *
 * Enable or disable the network for the machine
 */
void MachineConfigNetwork::saveNetworkData()
{
    bool useNetwork = false;

    if (this->m_withNetworkRadio->isChecked()) {
        useNetwork = true;
    }
    this->m_machine->setUseNetwork(useNetwork);
}

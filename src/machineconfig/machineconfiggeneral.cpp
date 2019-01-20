/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2019 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include "machineconfiggeneral.h"

/**
 * @brief Configuration of the machine. General page
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * Configuration of the machine. General page
 * with basic data, name, os version, os type,
 * state of the machine...
 */
MachineConfigGeneral::MachineConfigGeneral(Machine *machine,
                                           QWidget *parent) : QWidget(parent)
{
    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    this->m_machine = machine;

    m_basicTab = new BasicTab(machine, enableFields, this);
    m_descriptionTab = new DescriptionTab(machine, enableFields, this);

    m_generalTabWidget = new QTabWidget(this);
    m_generalTabWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                                            QSizePolicy::MinimumExpanding);
    m_generalTabWidget->addTab(this->m_basicTab, tr("Basic Details"));
    m_generalTabWidget->addTab(this->m_descriptionTab, tr("Description"));

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout->setAlignment(Qt::AlignCenter);
    m_generalPageLayout->addWidget(m_generalTabWidget);

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_generalPageWidget->setLayout(m_generalPageLayout);

    qDebug() << "MachineConfigGeneral created";
}

MachineConfigGeneral::~MachineConfigGeneral()
{
    qDebug() << "MachineConfigGeneral destroyed";
}

/**
 * @brief Save the general data
 *
 * Save the general data
 */
void MachineConfigGeneral::saveGeneralData()
{
    this->m_machine->setName(this->m_basicTab->getMachineName());
    this->m_machine->setOSType(this->m_basicTab->getMachineType());
    this->m_machine->setOSVersion(this->m_basicTab->getMachineVersion());
    this->m_machine->setDescription(this->m_descriptionTab->getMachineDescription());
}

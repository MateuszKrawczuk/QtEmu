// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
    m_customArgsTab = new CustomArgumentsTab(machine, enableFields, this);

    m_generalTabWidget = new QTabWidget(this);
    m_generalTabWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                                            QSizePolicy::MinimumExpanding);
    m_generalTabWidget->addTab(this->m_basicTab, tr("Basic Details"));
    m_generalTabWidget->addTab(this->m_descriptionTab, tr("Description"));
    m_generalTabWidget->addTab(this->m_customArgsTab, tr("Custom Arguments"));

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
    this->m_machine->setCustomArguments(this->m_customArgsTab->getCustomArguments());
}

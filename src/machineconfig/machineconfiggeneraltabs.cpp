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
#include "machineconfiggeneraltabs.h"

/**
 * @brief Tab with basic informacion about the machine
 * @param machine, machine to be configured
 * @param enableFields, fields enabled or disabled
 * @param parent, parent widget
 *
 * Tab with basic informacion about the machine.
 * Machine name, operating system type, version...
 */
BasicTab::BasicTab(Machine *machine,
                   bool enableFields,
                   QWidget *parent) : QWidget(parent)
{
    this->m_machineConfig = machine;

    m_machineNameLineEdit = new QLineEdit(this);
    m_machineNameLineEdit->setText(machine->getName());
    m_machineNameLineEdit->setEnabled(enableFields);

    m_OSType = new QComboBox(this);
    m_OSType->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::MinimumExpanding);
    m_OSType->setEnabled(enableFields);

    m_OSType->addItem("GNU/Linux");
    m_OSType->addItem("Microsoft Windows");
    m_OSType->addItem("BSD");
    m_OSType->addItem(tr("Other"));
    m_OSType->setCurrentText(machine->getOSType());

    connect(m_OSType, &QComboBox::currentTextChanged,
           this, &BasicTab::selectOS);

    m_OSVersion = new QComboBox(this);
    m_OSVersion->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::MinimumExpanding);
    m_OSVersion->setEnabled(enableFields);
    this->selectOS(machine->getOSType());
    m_OSVersion->setCurrentText(machine->getOSVersion());

    m_machineUuidLabel = new QLabel(this);
    m_machineUuidLabel->setText(machine->getUuid());
    m_machineStatusLabel = new QLabel(this);
    m_machineStatusLabel->setText(BasicTab::getStatusLabel(machine->getState()));

    m_basicTabFormLayout = new QFormLayout();
    m_basicTabFormLayout->setAlignment(Qt::AlignTop);
    m_basicTabFormLayout->setLabelAlignment(Qt::AlignLeft);
    m_basicTabFormLayout->setHorizontalSpacing(20);
    m_basicTabFormLayout->setVerticalSpacing(10);
    m_basicTabFormLayout->addRow(tr("Name") + ":", m_machineNameLineEdit);
    m_basicTabFormLayout->addRow(tr("Type") + ":", m_OSType);
    m_basicTabFormLayout->addRow(tr("Version") + ":", m_OSVersion);
    m_basicTabFormLayout->addRow(tr("UUID") + ":", m_machineUuidLabel);
    m_basicTabFormLayout->addRow(tr("Status") + ":", m_machineStatusLabel);

    m_basicTabLayout = new QVBoxLayout();
    m_basicTabLayout->addItem(m_basicTabFormLayout);

    this->setLayout(m_basicTabLayout);

    qDebug() << "BasicTab created";
}

BasicTab::~BasicTab()
{
    qDebug() << "BasicTab destroyed";
}

/**
 * @brief Update the data in the osversion combo
 * @param OSSelected, indicated the os type
 *
 * Update the data in the osversion combo.
 * If you select GNU/Linux, then charge all the GNU/Linux distributions
 */
void BasicTab::selectOS(QString OSSelected)
{
    this->m_OSVersion->clear();

    if (OSSelected == "GNU/Linux") {
        this->m_OSVersion->addItem(tr("Debian"));
        this->m_OSVersion->addItem(tr("Ubuntu"));
        this->m_OSVersion->addItem(tr("Fedora"));
        this->m_OSVersion->addItem(tr("OpenSuse"));
        this->m_OSVersion->addItem(tr("Mageia"));
        this->m_OSVersion->addItem(tr("Gentoo"));
        this->m_OSVersion->addItem(tr("Arch Linux"));
        this->m_OSVersion->addItem(tr("Linux"));
    } else if (OSSelected == "Microsoft Windows") {
        this->m_OSVersion->addItem(tr("Microsoft 95"));
        this->m_OSVersion->addItem(tr("Microsoft 98"));
        this->m_OSVersion->addItem(tr("Microsoft 2000"));
        this->m_OSVersion->addItem(tr("Microsoft XP"));
        this->m_OSVersion->addItem(tr("Microsoft Vista"));
        this->m_OSVersion->addItem(tr("Microsoft 7"));
        this->m_OSVersion->addItem(tr("Microsoft 8"));
        this->m_OSVersion->addItem(tr("Microsoft 10"));
    } else if (OSSelected == "BSD") {
        this->m_OSVersion->addItem(tr("FreeBSD"));
        this->m_OSVersion->addItem(tr("OpenBSD"));
        this->m_OSVersion->addItem(tr("NetBSD"));
    } else {
        this->m_OSVersion->addItem(tr("Debian GNU Hurd"));
        this->m_OSVersion->addItem(tr("Arch Hurd"));
        this->m_OSVersion->addItem(tr("Redox"));
        this->m_OSVersion->addItem(tr("ReactOS"));
    }
}

/**
 * @brief Get the status of the machine
 * @param state, state of the machine
 * @return status of the machine
 *
 * Get the status of the machine, transform the States enum
 * to a label
 */
QString BasicTab::getStatusLabel(Machine::States state)
{
    QString statusLabel;

    switch (state) {
        case Machine::Started:
            statusLabel = tr("Started");
            break;
        case Machine::Stopped:
            statusLabel = tr("Stopped");
            break;
        case Machine::Saved:
            statusLabel = tr("Saved");
            break;
        case Machine::Paused:
            statusLabel = tr("Paused");
            break;
        default:
            statusLabel = tr("Stopped");
            break;
    }

    return statusLabel;
}

/**
 * @brief Get the machine name
 * @return the machine name
 *
 * Get the name of the machine
 */
QString BasicTab::getMachineName() const
{
    return this->m_machineNameLineEdit->text();
}

/**
 * @brief Get the operating system type
 * @return operating system type
 *
 * Get the operating system type
 * Ex: GNU/Linux, *BSD, etc...
 */
QString BasicTab::getMachineType() const
{
    return this->m_OSType->currentText();
}

/**
 * @brief Get the operating system version
 * @return version of the operating system
 *
 * Get the opearting system version
 * Ex: Debian, Mageia, FreeBSD, OpenBSD...
 */
QString BasicTab::getMachineVersion() const
{
    return this->m_OSVersion->currentText();
}

/**
 * @brief Tab with the descripcion
 * @param machine, machine to be configured
 * @param enableFields, fields enabled or disabled
 * @param parent, parent widget
 *
 * Tab with the description of the machine
 */
DescriptionTab::DescriptionTab(Machine *machine,
                               bool enableFields,
                               QWidget *parent) : QWidget(parent)
{

    m_machineDescLabel = new QLabel(tr("Description") + ":", this);
    m_machineDescTextEdit = new QPlainTextEdit(this);
    m_machineDescTextEdit->setEnabled(enableFields);
    m_machineDescTextEdit->setPlainText(machine->getDescription());

    m_descriptionLayout = new QVBoxLayout();
    m_descriptionLayout->setAlignment(Qt::AlignVCenter);
    m_descriptionLayout->addWidget(m_machineDescLabel);
    m_descriptionLayout->addWidget(m_machineDescTextEdit);

    this->setLayout(m_descriptionLayout);

    qDebug() << "DescriptionTab created";
}

DescriptionTab::~DescriptionTab()
{
    qDebug() << "DescriptionTab destroyed";
}

/**
 * @brief Get the description of the machine
 * @return the description of the machine
 *
 * Get the description of the machine
 */
QString DescriptionTab::getMachineDescription() const
{
    return this->m_machineDescTextEdit->toPlainText();
}

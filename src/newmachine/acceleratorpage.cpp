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
#include "acceleratorpage.h"

/**
 * @brief Machine Accelerator page
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Accelerator page section. In this page you can add or remove the accelerators
 * to the new machine
 *
 * Supported: KVM, XEN, HAXM, WHPX and TCG
 */
MachineAcceleratorPage::MachineAcceleratorPage(Machine *machine,
                                               QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine accelerator"));
    this->m_newMachine = machine;

    m_acceleratorGroup = new QButtonGroup(this);
    
    m_tcgRadio = new QRadioButton(tr("TCG (default)"), this);
    m_acceleratorGroup->addButton(m_tcgRadio);

    #ifdef Q_OS_LINUX
    m_kvmRadio = new QRadioButton(tr("KVM"), this);
    m_xenRadio = new QRadioButton(tr("XEN"), this);
    m_acceleratorGroup->addButton(m_kvmRadio);
    m_acceleratorGroup->addButton(m_xenRadio);
    #endif

    #ifdef Q_OS_WIN
    m_whpxRadio = new QRadioButton(tr("WHPX"), this);
    m_haxmRadio = new QRadioButton(tr("HAXM (deprecated)"), this);
    m_acceleratorGroup->addButton(m_whpxRadio);
    m_acceleratorGroup->addButton(m_haxmRadio);
    m_haxmRadio->setEnabled(false);
    m_haxmRadio->setToolTip(tr("HAXM is deprecated and not recommended for use"));
    
    QProcess process;
    process.start("powershell", QStringList() << "-Command" << 
                 "Get-WindowsOptionalFeature -Online -FeatureName HypervisorPlatform");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    
    if (output.contains("Enabled")) {
        m_whpxRadio->setChecked(true);
    } else {
        m_tcgRadio->setChecked(true);
    }
    #endif

    #ifdef Q_OS_MACOS
    m_hvfRadio = new QRadioButton(tr("HVF"), this);
    m_acceleratorGroup->addButton(m_hvfRadio);
    #endif

    if (!m_acceleratorGroup->checkedButton()) {
        m_tcgRadio->setChecked(true);
    }

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_tcgRadio);
    #ifdef Q_OS_LINUX
    layout->addWidget(m_kvmRadio);
    layout->addWidget(m_xenRadio);
    #endif
    #ifdef Q_OS_WIN
    layout->addWidget(m_whpxRadio);
    layout->addWidget(m_haxmRadio);
    #endif
    #ifdef Q_OS_MACOS
    layout->addWidget(m_hvfRadio);
    #endif

    this->setLayout(layout);

    connect(m_acceleratorGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MachineAcceleratorPage::updateAccelerator);

    qDebug() << "MachineAcceleratorPage created";
}

MachineAcceleratorPage::~MachineAcceleratorPage()
{
    qDebug() << "MachineAcceleratorPage destroyed";
}

void MachineAcceleratorPage::updateAccelerator(QAbstractButton *button)
{
    m_newMachine->removeAllAccelerators();
    
    if (button == m_tcgRadio) {
        m_newMachine->addAccelerator("tcg");
    }
    #ifdef Q_OS_LINUX
    else if (m_kvmRadio && button == m_kvmRadio) {
        m_newMachine->addAccelerator("kvm");
    }
    else if (m_xenRadio && button == m_xenRadio) {
        m_newMachine->addAccelerator("xen");
    }
    #endif
    #ifdef Q_OS_WIN
    else if (m_whpxRadio && button == m_whpxRadio) {
        m_newMachine->addAccelerator("whpx"); 
    }
    #endif
    #ifdef Q_OS_MACOS
    else if (m_hvfRadio && button == m_hvfRadio) {
        m_newMachine->addAccelerator("hvf");
    }
    #endif
}

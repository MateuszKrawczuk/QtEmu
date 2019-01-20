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
#include "acceleratorpage.h"

/**
 * @brief Machine Accelerator page
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Accelerator page section. In this page you can add or remove the accelerators
 * to the new machine
 *
 * Supported: KVM, XEN, HAXM and TCG
 */
MachineAcceleratorPage::MachineAcceleratorPage(Machine *machine,
                                               QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine accelerator"));
    this->m_newMachine = machine;

    m_acceleratorTabWidget = new QTabWidget();

    #ifdef Q_OS_LINUX
    m_acceleratorTabWidget->addTab(new KVMTab(machine, this), tr("KVM"));
    m_acceleratorTabWidget->addTab(new XENTab(machine, this), tr("XEN"));
    #endif
    #ifdef Q_OS_WIN
    m_acceleratorTabWidget->addTab(new HAXMTab(machine, this), tr("HAXM"));
    #endif

    m_acceleratorTabWidget->addTab(new TCGTab(machine, this), tr("TCG"));

    m_acceleratorLayout = new QVBoxLayout();
    m_acceleratorLayout->setAlignment(Qt::AlignCenter);
    m_acceleratorLayout->addWidget(m_acceleratorTabWidget);

    this->setLayout(m_acceleratorLayout);

    qDebug() << "MachineAcceleratorPage created";
}

MachineAcceleratorPage::~MachineAcceleratorPage()
{
    qDebug() << "MachineAcceleratorPage destroyed";
}

/**
 * @brief KVMTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * KVMTab tab. In this tab you can add or remove the kvm accelerator
 */
KVMTab::KVMTab(Machine *machine, QWidget *parent) : QWidget(parent)
{
    this->m_newMachine = machine;
    this->addKVMAccelerator(true);

    m_kvmRadioButton = new QRadioButton("Kernel-based Virtual Machine (KVM)", this);
    m_kvmRadioButton->setChecked(true);

    connect(m_kvmRadioButton, &QAbstractButton::toggled,
                this, &KVMTab::addKVMAccelerator);

    m_kvmDescriptionLabel = new QLabel("KVM (for Kernel-based Virtual Machine) is a full virtualization solution"
                                     "for GNU/Linux on x86 hardware containing virtualization extensions (Intel VT or AMD-V).", this);
    m_kvmDescriptionLabel->setWordWrap(true);

    m_kvmURLLabel = new QLabel("<a href=\"https://www.linux-kvm.org\">www.linux-kvm.org</a>", this);

    m_kvmLayout = new QVBoxLayout();
    m_kvmLayout->addWidget(m_kvmRadioButton);
    m_kvmLayout->addWidget(m_kvmDescriptionLabel);
    m_kvmLayout->addWidget(m_kvmURLLabel, 0, Qt::AlignCenter);

    this->setLayout(m_kvmLayout);

    qDebug() << "KVMTab created";
}

KVMTab::~KVMTab()
{
    qDebug() << "KVMTab destroyed";
}

/**
 * @brief Add or remove the kvm accelerator
 * @param kvmAccelerator, true add the accelerator
 *
 * Add or remove the kvm accelerator
 */
void KVMTab::addKVMAccelerator(bool kvmAccelerator)
{
    if (kvmAccelerator) {
        this->m_newMachine->addAccelerator("kvm");
    } else {
        this->m_newMachine->removeAccelerator("kvm");
    }
}

/**
 * @brief XENTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * XENTab tab. In this tab you can add or remove the xen accelerator
 */
XENTab::XENTab(Machine *machine, QWidget *parent) : QWidget(parent)
{
    this->m_newMachine = machine;

    m_xenRadioButton = new QRadioButton("Xen Hypervisor", this);

    connect(m_xenRadioButton, &QAbstractButton::toggled,
                this, &XENTab::addXENAccelerator);

    m_xenDescriptionLabel = new QLabel("The Xen Project hypervisor is an open-source type-1 or "
                                       "baremetal hypervisor, which makes it possible to run many "
                                       "instances of an operating system or indeed different operating "
                                       "systems in parallel on a single machine (or host)", this);
    m_xenDescriptionLabel->setWordWrap(true);

    m_xenURLLabel = new QLabel("<a href=\"https://https://www.xenproject.org/\">www.xenproject.org</a>", this);

    m_xenLayout = new QVBoxLayout();
    m_xenLayout->addWidget(m_xenRadioButton);
    m_xenLayout->addWidget(m_xenDescriptionLabel);
    m_xenLayout->addWidget(m_xenURLLabel, 0, Qt::AlignCenter);

    this->setLayout(m_xenLayout);

    qDebug() << "XENTab created";
}

XENTab::~XENTab()
{
    qDebug() << "XENTab destroyed";
}

/**
 * @brief Add or remove the xen accelerator
 * @param xenAccelerator, true add the accelerator
 *
 * Add or remove the xen accelerator
 */
void XENTab::addXENAccelerator(bool xenAccelerator)
{
    if (xenAccelerator) {
        this->m_newMachine->addAccelerator("xen");
    } else {
        this->m_newMachine->removeAccelerator("xen");
    }
}

/**
 * @brief TCGTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * TCGTab tab. In this tab you can add or remove the tcg accelerator
 */
TCGTab::TCGTab(Machine *machine, QWidget *parent) : QWidget(parent)
{
    this->m_newMachine = machine;

    m_tcgRadioButton = new QRadioButton("Tiny Code Generator (TCG)", this);

    connect(m_tcgRadioButton, &QAbstractButton::toggled,
                this, &TCGTab::addTCGAccelerator);

    m_tcgDescriptionLabel = new QLabel("The Tiny Code Generator (TCG) exists to transform"
                                       "target insns (the processor being emulated) via the"
                                       "TCG frontend to TCG ops which are then transformed"
                                       "into host insns (the processor executing QEMU itself)"
                                       "via the TCG backend.", this);
    m_tcgDescriptionLabel->setWordWrap(true);

    m_tcgURLLabel = new QLabel("<a href=\"https://https://wiki.qemu.org/Documentation/TCG\">wiki.qemu.org</a>", this);

    m_tcgLayout = new QVBoxLayout();
    m_tcgLayout->addWidget(m_tcgRadioButton);
    m_tcgLayout->addWidget(m_tcgDescriptionLabel);
    m_tcgLayout->addWidget(m_tcgURLLabel, 0, Qt::AlignCenter);

    this->setLayout(m_tcgLayout);

    qDebug() << "TCGTab created";
}

TCGTab::~TCGTab()
{
    qDebug() << "TCGTab destroyed";
}

/**
 * @brief Add or remove the tcg accelerator
 * @param tcgAccelerator, true add the accelerator
 *
 * Add or remove the tcg accelerator
 */
void TCGTab::addTCGAccelerator(bool tcgAccelerator)
{
    if (tcgAccelerator) {
        this->m_newMachine->addAccelerator("tcg");
    } else {
        this->m_newMachine->removeAccelerator("tcg");
    }
}

/**
 * @brief HAXMTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * HAXMTab tab. In this tab you can add or remove the haxm accelerator
 */
HAXMTab::HAXMTab(Machine *machine, QWidget *parent) : QWidget(parent)
{
    this->m_newMachine = machine;
    #ifdef Q_OS_WIN
    this->addHAXAccelerator(true);
    #endif

    m_haxmRadioButton = new QRadioButton("Hardware Accelerated Execution Manager (HAXM)", this);
    m_haxmRadioButton->setChecked(true);

    connect(m_haxmRadioButton, &QAbstractButton::toggled,
                this, &HAXMTab::addHAXAccelerator);

    m_haxmDescriptionLabel = new QLabel("Intel® Hardware Accelerated Execution Manager"
                                        "(Intel® HAXM) is a hardware-assisted virtualization"
                                        "engine (hypervisor) that uses Intel® Virtualization Technology"
                                        "(Intel® VT) to speed up Android* app emulation on a host machine.", this);
    m_haxmDescriptionLabel->setWordWrap(true);

    m_haxmURLLabel = new QLabel("<a href=\"https://software.intel.com/en-us/articles/intel-hardware-accelerated-execution-manager-intel-haxm\">software.intel.com</a>", this);

    m_haxmLayout = new QVBoxLayout();
    m_haxmLayout->addWidget(m_haxmRadioButton);
    m_haxmLayout->addWidget(m_haxmDescriptionLabel);
    m_haxmLayout->addWidget(m_haxmURLLabel, 0, Qt::AlignCenter);

    this->setLayout(m_haxmLayout);

    qDebug() << "HAXMTab created";
}

HAXMTab::~HAXMTab()
{
    qDebug() << "HAXMTab destroyed";
}

/**
 * @brief Add or remove the hax accelerator
 * @param haxAccelerator, true add the accelerator
 *
 * Add or remove the hax accelerator
 */
void HAXMTab::addHAXAccelerator(bool haxAccelerator)
{
    if (haxAccelerator) {
        this->m_newMachine->addAccelerator("hax");
    } else {
        this->m_newMachine->removeAccelerator("hax");
    }
}

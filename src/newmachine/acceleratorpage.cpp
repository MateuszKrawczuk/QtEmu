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
#include "acceleratorpage.h"

MachineAcceleratorPage::MachineAcceleratorPage(Machine *machine,
                                               QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine accelerator"));

    this -> newMachine = machine;

    acceleratorTabWidget = new QTabWidget();

    #ifdef Q_OS_LINUX
    acceleratorTabWidget -> addTab(new KVMTab(machine, this), tr("KVM"));
    acceleratorTabWidget -> addTab(new XENTab(machine, this), tr("XEN"));
    #endif

    acceleratorTabWidget -> addTab(new TCGTab(machine, this), tr("TCG"));

    #ifdef Q_OS_WIN
    acceleratorTabWidget -> addTab(new HAXMTab(machine, this), tr("HAXM"));
    #endif

    acceleratorLayout = new QVBoxLayout();
    acceleratorLayout -> setAlignment(Qt::AlignCenter);
    acceleratorLayout -> addWidget(acceleratorTabWidget);

    setLayout(acceleratorLayout);

    qDebug() << "MachineAcceleratorPage created";
}

MachineAcceleratorPage::~MachineAcceleratorPage() {
    qDebug() << "MachineAcceleratorPage destroyed";
}

KVMTab::KVMTab(Machine *machine,
               QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    kvmRadioButton = new QRadioButton("Kernel-based Virtual Machine (KVM)");
    kvmRadioButton -> setChecked(true);

    this -> addKVMAccelerator(true);

    connect(kvmRadioButton, &QAbstractButton::toggled,
                this, &KVMTab::addKVMAccelerator);

    kvmDescriptionLabel = new QLabel("KVM (for Kernel-based Virtual Machine) is a full virtualization solution"
                                     "for GNU/Linux on x86 hardware containing virtualization extensions (Intel VT or AMD-V).");
    kvmDescriptionLabel -> setWordWrap(true);

    kvmURLLabel = new QLabel("<a href=\"https://www.linux-kvm.org\">www.linux-kvm.org</a>");

    kvmLayout = new QVBoxLayout();
    kvmLayout -> addWidget(kvmRadioButton);
    kvmLayout -> addWidget(kvmDescriptionLabel);
    kvmLayout -> addWidget(kvmURLLabel, 0, Qt::AlignCenter);

    setLayout(kvmLayout);

    qDebug() << "KVMTab created";
}

KVMTab::~KVMTab() {
    qDebug() << "KVMTab destroyed";
}

void KVMTab::addKVMAccelerator(bool kvmAccelerator) {
    if(kvmAccelerator) {
        this -> newMachine -> addAccelerator("kvm", "kvm");
    } else {
        this -> newMachine -> removeAccelerator("kvm");
    }
}

XENTab::XENTab(Machine *machine,
               QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    xenRadioButton = new QRadioButton("Xen Hypervisor");

    connect(xenRadioButton, &QAbstractButton::toggled,
                this, &XENTab::addXENAccelerator);

    xenDescriptionLabel = new QLabel("The Xen Project hypervisor is an open-source type-1 or "
                                     "baremetal hypervisor, which makes it possible to run many "
                                     "instances of an operating system or indeed different operating "
                                     "systems in parallel on a single machine (or host)");
    xenDescriptionLabel -> setWordWrap(true);

    xenURLLabel = new QLabel("<a href=\"https://https://www.xenproject.org/\">www.xenproject.org</a>");

    xenLayout = new QVBoxLayout();
    xenLayout -> addWidget(xenRadioButton);
    xenLayout -> addWidget(xenDescriptionLabel);
    xenLayout -> addWidget(xenURLLabel, 0, Qt::AlignCenter);

    setLayout(xenLayout);

    qDebug() << "XENTab created";
}

XENTab::~XENTab() {
    qDebug() << "XENTab destroyed";
}

void XENTab::addXENAccelerator(bool xenAccelerator) {
    if(xenAccelerator) {
        this -> newMachine -> addAccelerator("xen", "xen");
    } else {
        this -> newMachine -> removeAccelerator("xen");
    }
}

TCGTab::TCGTab(Machine *machine,
               QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    tcgRadioButton = new QRadioButton("Tiny Code Generator (TCG)");

    connect(tcgRadioButton, &QAbstractButton::toggled,
                this, &TCGTab::addTCGAccelerator);

    tcgDescriptionLabel = new QLabel("The Tiny Code Generator (TCG) exists to transform"
                                      "target insns (the processor being emulated) via the"
                                      "TCG frontend to TCG ops which are then transformed"
                                      "into host insns (the processor executing QEMU itself)"
                                      "via the TCG backend.");
    tcgDescriptionLabel -> setWordWrap(true);

    tcgURLLabel = new QLabel("<a href=\"https://https://wiki.qemu.org/Documentation/TCG\">wiki.qemu.org</a>");

    tcgLayout = new QVBoxLayout();
    tcgLayout -> addWidget(tcgRadioButton);
    tcgLayout -> addWidget(tcgDescriptionLabel);
    tcgLayout -> addWidget(tcgURLLabel, 0, Qt::AlignCenter);

    setLayout(tcgLayout);

    qDebug() << "TCGTab created";
}

TCGTab::~TCGTab() {
    qDebug() << "TCGTab destroyed";
}

void TCGTab::addTCGAccelerator(bool tcgAccelerator) {
    if(tcgAccelerator) {
        this -> newMachine -> addAccelerator("tcg", "tcg");
    } else {
        this -> newMachine -> removeAccelerator("tcg");
    }
}

HAXMTab::HAXMTab(Machine *machine,
                 QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    haxmRadioButton = new QRadioButton("Hardware Accelerated Execution Manager (HAXM)");

    connect(haxmRadioButton, &QAbstractButton::toggled,
                this, &HAXMTab::addHAXAccelerator);

    haxmDescriptionLabel = new QLabel("Intel® Hardware Accelerated Execution Manager"
                                       "(Intel® HAXM) is a hardware-assisted virtualization"
                                       "engine (hypervisor) that uses Intel® Virtualization Technology"
                                       "(Intel® VT) to speed up Android* app emulation on a host machine."
                                      );
    haxmDescriptionLabel -> setWordWrap(true);

    haxmURLLabel = new QLabel("<a href=\"https://software.intel.com/en-us/articles/intel-hardware-accelerated-execution-manager-intel-haxm\">software.intel.com</a>");

    haxmLayout = new QVBoxLayout();
    haxmLayout -> addWidget(haxmRadioButton);
    haxmLayout -> addWidget(haxmDescriptionLabel);
    haxmLayout -> addWidget(haxmURLLabel, 0, Qt::AlignCenter);

    setLayout(haxmLayout);

    qDebug() << "HAXMTab created";
}

HAXMTab::~HAXMTab() {
    qDebug() << "HAXMTab destroyed";
}

void HAXMTab::addHAXAccelerator(bool haxAccelerator) {
    if(haxAccelerator) {
        this -> newMachine -> addAccelerator("hax", "hax");
    } else {
        this -> newMachine -> removeAccelerator("hax");
    }
}

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
#include "machinewizard.h"

MachineWizard::MachineWizard(QWidget *parent) : QWizard(parent) {

    setWindowTitle(tr("Create a new Machine"));

    setPage(Page_Name, new MachineNamePage(this));
    setPage(Page_Hardware, new MachineHardwarePage(this));
    setPage(Page_Accelerator, new MachineAcceleratorPage(this));
    setPage(Page_Memory, new MachineMemoryPage(this));
    setPage(Page_Disk, new MachineDiskPage(this));
    setPage(Page_Conclusion, new MachineConclusionPage(this));

    setStartId(Page_Name);

    #ifndef Q_OS_MAC
        setWizardStyle(ClassicStyle);
    #endif

    #ifdef Q_OS_MAC
        setWizardStyle(MacStyle);
    #endif

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    setMinimumSize(600, 400);

    qDebug() << "MachineWizard created";
}

MachineWizard::~MachineWizard() {
    qDebug() << "MachineWizard destroyed";
}

MachineNamePage::MachineNamePage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine name and operating system"));

    descriptionNameLabel = new QLabel(tr("Select name and operating system for your new machine."));
    descriptionNameLabel -> setWordWrap(true);

    machineNameLabel = new QLabel(tr("Name") + ":");
    machineNameLineEdit = new QLineEdit();

    OSTypeLabel = new QLabel(tr("Type") + ":");
    OSType = new QComboBox();

    OSType -> addItem(tr("GNU/Linux"));
    OSType -> addItem(tr("Microsoft Windows"));
    OSType -> addItem(tr("BSD"));
    OSType -> addItem(tr("Other"));

    connect(OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MachineNamePage::selectOS);

    OSVersionLabel = new QLabel(tr("Version") + ":");
    OSVersion = new QComboBox();

    selectOS(0);

    mainLayout = new QGridLayout();

    mainLayout -> setColumnStretch(1, 10);
    mainLayout -> setColumnStretch(2, 10);
    mainLayout -> setColumnStretch(3, 10);

    mainLayout -> addWidget(descriptionNameLabel, 0, 0, 1, 4);
    mainLayout -> addWidget(machineNameLabel,     1, 0, 1, 1);
    mainLayout -> addWidget(machineNameLineEdit,  1, 1, 1, 3);
    mainLayout -> addWidget(OSTypeLabel,          2, 0, 1, 1);
    mainLayout -> addWidget(OSType,               2, 1, 1, 3);
    mainLayout -> addWidget(OSVersionLabel,       3, 0, 1, 1);
    mainLayout -> addWidget(OSVersion,            3, 1, 1, 3);

    setLayout(mainLayout);

    qDebug() << "MachineNamePage created";
}

MachineNamePage::~MachineNamePage() {
    qDebug() << "MachineNamePage destroyed";
}

MachineHardwarePage::MachineHardwarePage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine hardware"));

    qDebug() << "MachineHardwarePage created";
}

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
}

MachineAcceleratorPage::MachineAcceleratorPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine accelerator"));

    acceleratorTabWidget = new QTabWidget();
    acceleratorTabWidget -> addTab(new KVMTab(), tr("KVM"));
    acceleratorTabWidget -> addTab(new TCGTab(), tr("TCG"));
    acceleratorTabWidget -> addTab(new HAXMTab(), tr("HAXM"));

    acceleratorLayout = new QVBoxLayout();
    acceleratorLayout -> setAlignment(Qt::AlignCenter);
    acceleratorLayout -> addWidget(acceleratorTabWidget);

    setLayout(acceleratorLayout);

    qDebug() << "MachineAcceleratorPage created";
}

MachineAcceleratorPage::~MachineAcceleratorPage() {
    qDebug() << "MachineAcceleratorPage destroyed";
}

KVMTab::KVMTab(QWidget *parent) : QWidget(parent) {

    kvmRadioButton = new QRadioButton("Kernel-based Virtual Machine (KVM)");

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

TCGTab::TCGTab(QWidget *parent) : QWidget(parent) {

    tcgRadioButton = new QRadioButton("Tiny Code Generator (TCG)");

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

HAXMTab::HAXMTab(QWidget *parent) : QWidget(parent) {

    haxmRadioButton = new QRadioButton("Hardware Accelerated Execution Manager (HAXM)");

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

MachineMemoryPage::MachineMemoryPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine memory"));

    descriptionMemoryLabel = new QLabel(
                tr("Select the amount of base memory (RAM) in megabytes for virtual machine allocating."));
    descriptionMemoryLabel -> setWordWrap(true);

    int32_t totalRAM = 0;

    SystemUtils::getTotalMemory(totalRAM);

    spinBoxMemoryLabel = new QLabel("MiB");

    memorySpinBox = new QSpinBox();
    memorySpinBox -> setMinimum(1);
    memorySpinBox -> setMaximum(totalRAM);

    memorySlider = new QSlider(Qt::Horizontal);
    memorySlider -> setTickPosition(QSlider::TicksBelow);
    memorySlider -> setTickInterval(500);
    memorySlider -> setMinimum(1);
    memorySlider -> setMaximum(totalRAM);

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

MachineDiskPage::MachineDiskPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr(""));
}

MachineDiskPage::~MachineDiskPage() {
    qDebug() << "MachineDiskPage destroyed";
}

MachineConclusionPage::MachineConclusionPage(QWidget *parent) : QWizardPage(parent) {

}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

void MachineNamePage::selectOS(int OSSelected){

    this -> OSVersion -> clear();

    if (OSSelected == 0) {
        this -> OSVersion -> addItem(tr("Debian"));
        this -> OSVersion -> addItem(tr("Ubuntu"));
        this -> OSVersion -> addItem(tr("Fedora"));
        this -> OSVersion -> addItem(tr("OpenSuse"));
        this -> OSVersion -> addItem(tr("Mageia"));
        this -> OSVersion -> addItem(tr("Gentoo"));
        this -> OSVersion -> addItem(tr("Arch Linux"));
        this -> OSVersion -> addItem(tr("Linux 4.x"));
    } else if (OSSelected == 1) {
        this -> OSVersion -> addItem(tr("Microsoft 95"));
        this -> OSVersion -> addItem(tr("Microsoft 98"));
        this -> OSVersion -> addItem(tr("Microsoft 2000"));
        this -> OSVersion -> addItem(tr("Microsoft XP"));
        this -> OSVersion -> addItem(tr("Microsoft Vista"));
        this -> OSVersion -> addItem(tr("Microsoft 7"));
        this -> OSVersion -> addItem(tr("Microsoft 8"));
        this -> OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == 2) {
        this -> OSVersion -> addItem(tr("FreeBSD"));
        this -> OSVersion -> addItem(tr("OpenBSD"));
        this -> OSVersion -> addItem(tr("NetBSD"));
    } else if (OSSelected == 3) {
        this -> OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> OSVersion -> addItem(tr("Arch Hurd"));
        this -> OSVersion -> addItem(tr("Minix"));
        this -> OSVersion -> addItem(tr("ReactOS"));
    }
}

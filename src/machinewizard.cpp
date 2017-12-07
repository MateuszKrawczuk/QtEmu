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

MachineWizard::MachineWizard(Machine *machine,
                             QWidget *parent) : QWizard(parent) {

    setWindowTitle(tr("Create a new Machine"));

    setPage(Page_Name, new MachineNamePage(machine, this));
    setPage(Page_Hardware, new MachineHardwarePage(machine, this));
    setPage(Page_Accelerator, new MachineAcceleratorPage(machine, this));
    setPage(Page_Memory, new MachineMemoryPage(machine, this));
    setPage(Page_Disk, new MachineDiskPage(machine, this));
    setPage(Page_New_Disk, new MachineNewDiskPage(machine ,this));

    setStartId(Page_Name);

    #ifndef Q_OS_MAC
        setWizardStyle(ClassicStyle);
    #endif

    #ifdef Q_OS_MAC
        setWizardStyle(MacStyle);
    #endif

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    setMinimumSize(700, 400);

    qDebug() << "MachineWizard created";
}

MachineWizard::~MachineWizard() {
    qDebug() << "MachineWizard destroyed";
}

/*void MachineWizard::createMachineJSON(QJsonObject &machineJson) const {

}*/

MachineNamePage::MachineNamePage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    this -> setTitle(tr("Machine name and operating system"));

    this -> newMachine = machine;

    descriptionNameLabel = new QLabel(tr("Select name and operating system for your new machine."));
    descriptionNameLabel -> setWordWrap(true);

    machineNameLabel = new QLabel(tr("Name") + ":");
    machineNameLineEdit = new QLineEdit();

    OSTypeLabel = new QLabel(tr("Type") + ":");
    OSType = new QComboBox();

    OSType -> addItem("GNU/Linux");
    OSType -> addItem("Microsoft Windows");
    OSType -> addItem("BSD");
    OSType -> addItem(tr("Other"));

    connect(OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MachineNamePage::selectOS);

    OSVersionLabel = new QLabel(tr("Version") + ":");
    OSVersion = new QComboBox();

    this -> selectOS(0);

    this -> registerField("machine.name*", machineNameLineEdit);
    this -> registerField("machine.ostype", OSType, "currentText", "currentTextChanged");
    this -> registerField("machine.osversion", OSVersion, "currentText", "currentTextChanged");

    machineFolderCreated = QString();

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

    this ->  setLayout(mainLayout);

    qDebug() << "MachineNamePage created";
}

MachineNamePage::~MachineNamePage() {
    qDebug() << "MachineNamePage destroyed";
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

bool MachineNamePage::validatePage() {

    if ( ! machineFolderCreated.isEmpty() ) {
        return true;
    }

    QSettings settings;
    settings.beginGroup("Configuration");

    QString strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

    settings.endGroup();

    QString strMachineName = field("machine.name").toString();
    QString strMachinePathMsg = strMachinePath;
    QString strFullMachinePath = strMachinePath.append("/").append(strMachineName);

    if ( QDir(strFullMachinePath).exists() ) {
        qDebug() << "The folder alredy exists" << strFullMachinePath;

        createMachineMessageBox = new QMessageBox();
        createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        createMachineMessageBox -> setIcon(QMessageBox::Critical);
        createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                              "in the parent folder <strong>%2<strong></p>"
                                              "<p>This folder alredy exists and possibly belongs to another machine.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        createMachineMessageBox -> exec();

        return false;
    }

    if ( ! QDir().mkpath(strFullMachinePath) ) {
        qDebug() << "Machine folder created" << strFullMachinePath;

        createMachineMessageBox = new QMessageBox();
        createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        createMachineMessageBox -> setIcon(QMessageBox::Critical);
        createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                              "in the parent folder <strong>%2<strong></p>"
                                              "<p>Please check that the parent really exists and that "
                                              "you have permissions to create the machine folder.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        createMachineMessageBox->exec();

        return false;
    }

    machineFolderCreated = strFullMachinePath;

    // Set all the values in the machine object
    this -> newMachine -> setName(this -> machineNameLineEdit -> text());
    this -> newMachine -> setOSType(this -> OSType -> currentText());
    this -> newMachine -> setOSVersion(this -> OSVersion -> currentText());

    return true;
}

void MachineNamePage::cleanupPage() {

    if ( ! machineFolderCreated.isEmpty() ) {
        QDir().rmpath(machineFolderCreated);
    }

}

MachineHardwarePage::MachineHardwarePage(Machine *machine,
                                         QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine hardware"));

    this -> newMachine = machine;

    hardwareTabWidget = new QTabWidget();
    hardwareTabWidget -> addTab(new ProcessorTab(machine, this), tr("Processor"));
    hardwareTabWidget -> addTab(new GraphicsTab(machine, this), tr("Graphics"));
    hardwareTabWidget -> addTab(new AudioTab(machine, this), tr("Audio"));
    hardwareTabWidget -> addTab(new NetworkTab(machine, this), tr("Network"));

    hardwareLayout = new QVBoxLayout();
    hardwareLayout -> setAlignment(Qt::AlignCenter);
    hardwareLayout -> addWidget(hardwareTabWidget);      

    setLayout(hardwareLayout);

    qDebug() << "MachineHardwarePage created";
}

ProcessorTab::ProcessorTab(Machine *machine,
                           QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    CPUTypeLabel = new QLabel(tr("CPU Type") + ":");
    CPUTypeLabel -> setWordWrap(true);

    CPUType = new QComboBox();
    SystemUtils::setCPUTypesx86(CPUType);

    this -> selectProcessor(this -> CPUType -> itemText(0));

    connect(CPUType, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &ProcessorTab::selectProcessor);

    CPUTypeLayout = new QHBoxLayout();
    CPUTypeLayout -> setAlignment(Qt::AlignVCenter);
    CPUTypeLayout -> setSpacing(5);
    CPUTypeLayout -> addWidget(CPUTypeLabel);
    CPUTypeLayout -> addWidget(CPUType);

    CPUCountLabel = new QLabel(tr("CPU Count") + ":");
    CPUCountLabel -> setWordWrap(true);
    CPUCountSpinBox = new QSpinBox();
    CPUCountSpinBox -> setMinimum(1);
    CPUCountSpinBox -> setMaximum(255);

    this -> selectCPUCount(1);
    connect(CPUCountSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectCPUCount);

    coresSocketLabel = new QLabel(tr("Cores per socket") + ":");
    coresSocketLabel -> setWordWrap(true);
    coresSocketSpinBox = new QSpinBox();
    coresSocketSpinBox -> setMinimum(0);
    coresSocketSpinBox -> setMaximum(255);

    connect(coresSocketSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectCoresSocket);

    CPUCountLayout = new QHBoxLayout();
    CPUCountLayout -> setAlignment(Qt::AlignVCenter);
    CPUCountLayout -> setSpacing(5);
    CPUCountLayout -> addWidget(CPUCountLabel);
    CPUCountLayout -> addWidget(CPUCountSpinBox);
    CPUCountLayout -> addWidget(coresSocketLabel);
    CPUCountLayout -> addWidget(coresSocketSpinBox);

    socketCountLabel = new QLabel(tr("Socket count") + ":");
    socketCountLabel -> setWordWrap(true);
    socketCountSpinBox = new QSpinBox();
    socketCountSpinBox -> setMinimum(0);
    socketCountSpinBox -> setMaximum(255);

    connect(socketCountSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectSocketCount);

    threadsCoreLabel = new QLabel(tr("Threads per core") + ":");
    threadsCoreLabel -> setWordWrap(true);
    threadsCoreSpinBox = new QSpinBox();
    threadsCoreSpinBox -> setMinimum(0);
    threadsCoreSpinBox -> setMaximum(255);

    connect(threadsCoreSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectThreadsCore);

    socketLayout = new QHBoxLayout();
    socketLayout -> setAlignment(Qt::AlignVCenter);
    socketLayout -> setSpacing(5);
    socketLayout -> addWidget(socketCountLabel);
    socketLayout -> addWidget(socketCountSpinBox);
    socketLayout -> addWidget(threadsCoreLabel);
    socketLayout -> addWidget(threadsCoreSpinBox);

    maxHotCPULabel = new QLabel(tr("Maximum number of hotpluggable CPUs") + ":");
    maxHotCPULabel -> setWordWrap(false);
    maxHotCPUSpinBox = new QSpinBox();
    maxHotCPUSpinBox -> setMinimum(0);
    maxHotCPUSpinBox -> setMaximum(255);

    connect(maxHotCPUSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectMaxHotCPU);

    maxHotCPUsLayout = new QHBoxLayout();
    maxHotCPUsLayout -> setAlignment(Qt::AlignVCenter);
    maxHotCPUsLayout -> setSpacing(5);
    maxHotCPUsLayout -> addWidget(maxHotCPULabel);
    maxHotCPUsLayout -> addWidget(maxHotCPUSpinBox);

    CPUSettingsLayout = new QVBoxLayout();
    CPUSettingsLayout -> setAlignment(Qt::AlignVCenter);
    CPUSettingsLayout -> addItem(CPUCountLayout);
    CPUSettingsLayout -> addItem(socketLayout);
    CPUSettingsLayout -> addItem(maxHotCPUsLayout);

    CPUSettings = new QGroupBox(tr("CPU Settings"));
    CPUSettings -> setLayout(CPUSettingsLayout);

    processorLayout = new QVBoxLayout();
    processorLayout -> setAlignment(Qt::AlignVCenter);
    processorLayout -> addItem(CPUTypeLayout);
    processorLayout -> addWidget(CPUSettings);

    setLayout(processorLayout);

    qDebug() << "ProcessorTab created";
}

ProcessorTab::~ProcessorTab() {
    qDebug() << "ProcessorTab destroyed";
}

void ProcessorTab::selectProcessor(const QString &processor) {
    this -> newMachine -> setCPUType(processor);
}

void ProcessorTab::selectCPUCount(int CPUCount) {
    this -> newMachine -> setCPUCount(CPUCount);
}

void ProcessorTab::selectSocketCount(int socketCount) {
    this -> newMachine -> setSocketCount(socketCount);
}

void ProcessorTab::selectCoresSocket(int coresSocket) {
    this -> newMachine -> setCoresSocket(coresSocket);
}

void ProcessorTab::selectThreadsCore(int threadsCore) {
    this -> newMachine -> setThreadsCore(threadsCore);
}

void ProcessorTab::selectMaxHotCPU(int maxCPU) {
    this -> newMachine -> setMaxHotCPU(maxCPU);
}

GraphicsTab::GraphicsTab(Machine *machine,
                         QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    GPUTypeLabel = new QLabel(tr("GPU Type") + ":");
    GPUTypeLabel -> setWordWrap(true);
    GPUType = new QComboBox();
    SystemUtils::setGPUTypes(GPUType);

    GPUType -> setCurrentIndex(1);

    this -> selectGraphics(this -> GPUType -> itemText(0));

    connect(GPUType, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &GraphicsTab::selectGraphics);

    keyboardLabel = new QLabel(tr("Keyboard layout") + ":");
    keyboardLabel -> setWordWrap(true);
    keyboard = new QComboBox();
    SystemUtils::setKeyboardLayout(keyboard);

    keyboard -> setCurrentIndex(5);

    this -> selectKeyboard(this -> keyboard -> itemText(5));

    connect(keyboard, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &GraphicsTab::selectKeyboard);

    graphicsLayout = new QGridLayout();

    graphicsLayout -> setColumnStretch(1, 10);
    graphicsLayout -> setColumnStretch(2, 10);

    graphicsLayout -> addWidget(GPUTypeLabel,  1, 0, 1, 1);
    graphicsLayout -> addWidget(GPUType,       1, 1, 1, 3);
    graphicsLayout -> addWidget(keyboardLabel, 2, 0, 1, 1);
    graphicsLayout -> addWidget(keyboard,      2, 1, 1, 3);

    setLayout(graphicsLayout);

    qDebug() << "GraphicsTab created";
}

GraphicsTab::~GraphicsTab() {
    qDebug() << "GraphicsTab destroyed";
}

void GraphicsTab::selectGraphics(const QString &GPUType) {
    this -> newMachine -> setGPUType(GPUType);
}

void GraphicsTab::selectKeyboard(const QString &keyboard) {
    this -> newMachine -> setKeyboard(keyboard);
}

AudioTab::AudioTab(Machine *machine,
                   QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    creativeCheck = new QCheckBox("Creative Sound Blaster 16");
    creativeCheck -> setChecked(true);
    connect(creativeCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectCreativeCard);

    ensoniqCheck = new QCheckBox("ENSONIQ AudioPCI ES1370");
    ensoniqCheck -> setChecked(true);
    connect(ensoniqCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectEnsoniqCard);

    intelAC97Check = new QCheckBox("Intel AC97(82801AA)");
    intelAC97Check -> setChecked(true);
    connect(intelAC97Check, &QAbstractButton::toggled,
                this, &AudioTab::selectIntelAC97Card);

    yamahaCheck = new QCheckBox("Yamaha YM3812");
    yamahaCheck -> setChecked(true);
    connect(yamahaCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectYamahaCard);

    gravisCheck = new QCheckBox("Gravis Ultrasound GF1");
    gravisCheck -> setChecked(true);
    connect(gravisCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectGravisCard);

    CS4231ACheck = new QCheckBox("CS4231A");
    CS4231ACheck -> setChecked(true);
    connect(CS4231ACheck, &QAbstractButton::toggled,
                this, &AudioTab::selectCS4231ACard);

    intelHDACheck = new QCheckBox("Intel HD Audio");
    intelHDACheck -> setChecked(true);
    connect(intelHDACheck, &QAbstractButton::toggled,
                this, &AudioTab::selectIntelHDCard);

    pcSpeakerCheck = new QCheckBox(tr("PC Speaker"));
    pcSpeakerCheck -> setChecked(true);
    connect(pcSpeakerCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectPCSpeakerCard);

    audioLayout = new QGridLayout();
    audioLayout -> addWidget(creativeCheck,  0, 0, 1, 1);
    audioLayout -> addWidget(ensoniqCheck,   0, 1, 1, 1);

    audioLayout -> addWidget(intelAC97Check, 1, 0, 1, 1);
    audioLayout -> addWidget(yamahaCheck,    1, 1, 1, 1);

    audioLayout -> addWidget(gravisCheck,    2, 0, 1, 1);
    audioLayout -> addWidget(CS4231ACheck,   2, 1, 1, 1);

    audioLayout -> addWidget(intelHDACheck,  3, 0, 1, 1);
    audioLayout -> addWidget(pcSpeakerCheck, 3, 1, 1, 1);

    setLayout(audioLayout);

    qDebug() << "AudioTab created";
}

AudioTab::~AudioTab() {
    qDebug() << "AudioTab destroyed";
}

void AudioTab::selectCreativeCard(bool selectCreative) {
    if (selectCreative) {
        this -> newMachine -> addAudio("sb16", "Creative Sound Blaster 16");
    } else {
        this -> newMachine -> removeAudio("sb16");
    }
}

void AudioTab::selectEnsoniqCard(bool selectEnsoniq) {
    if (selectEnsoniq) {
        this -> newMachine -> addAudio("es1370", "ENSONIQ AudioPCI ES1370");
    } else {
        this -> newMachine -> removeAudio("es1370");
    }
}

void AudioTab::selectIntelAC97Card(bool selectIntelAC97) {
    if (selectIntelAC97) {
        this -> newMachine -> addAudio("ac97", "Intel AC97(82801AA)");
    } else {
        this -> newMachine -> removeAudio("ac97");
    }
}

void AudioTab::selectYamahaCard(bool selectYamaha) {
    if (selectYamaha) {
        this -> newMachine -> addAudio("adlib", "Yamaha YM3812");
    } else {
        this -> newMachine -> removeAudio("adlib");
    }
}

void AudioTab::selectGravisCard(bool selectGravis) {
    if (selectGravis) {
        this -> newMachine -> addAudio("gus", "Gravis Ultrasound GF1");
    } else {
        this -> newMachine -> removeAudio("gus");
    }
}

void AudioTab::selectCS4231ACard(bool selectCS4231A) {
    if (selectCS4231A) {
        this -> newMachine -> addAudio("cs4231a", "CS4231A");
    } else {
        this -> newMachine -> removeAudio("cs4231a");
    }
}

void AudioTab::selectIntelHDCard(bool selectIntelHD) {
    if (selectIntelHD) {
        this -> newMachine -> addAudio("hda", "Intel HD Audio");
    } else {
        this -> newMachine -> removeAudio("hda");
    }
}

void AudioTab::selectPCSpeakerCard(bool selectPCSpeaker) {
    if (selectPCSpeaker) {
        this -> newMachine -> addAudio("pcspk", "PC Speaker");
    } else {
        this -> newMachine -> removeAudio("pcspk");
    }
}

NetworkTab::NetworkTab(Machine *machine,
                       QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

    withNetworkRadio = new QRadioButton(tr("User Mode Network Connection (Uses the user mode network stack)"));
    withNetworkRadio -> setChecked(true);

    withoutNetworkRadio = new QRadioButton(tr("No network (No network cards installed on this machine"));

    networkLayout = new QVBoxLayout();
    networkLayout -> addWidget(withNetworkRadio);
    networkLayout -> addWidget(withoutNetworkRadio);

    setLayout(networkLayout);

    qDebug() << "NetworkTab created";
}

NetworkTab::~NetworkTab() {
    qDebug() << "NetworkTab destroyed";
}

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
}

MachineAcceleratorPage::MachineAcceleratorPage(Machine *machine,
                                               QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine accelerator"));

    this -> newMachine = machine;

    acceleratorTabWidget = new QTabWidget();
    acceleratorTabWidget -> addTab(new KVMTab(machine, this), tr("KVM"));
    acceleratorTabWidget -> addTab(new TCGTab(machine, this), tr("TCG"));
    acceleratorTabWidget -> addTab(new HAXMTab(machine, this), tr("HAXM"));

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

TCGTab::TCGTab(Machine *machine,
               QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

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

HAXMTab::HAXMTab(Machine *machine,
                 QWidget *parent) : QWidget(parent) {

    this -> newMachine = machine;

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

MachineMemoryPage::MachineMemoryPage(Machine *machine,
                                     QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine memory"));

    this -> newMachine = machine;

    descriptionMemoryLabel = new QLabel(
                tr("Select the amount of base memory (RAM) in megabytes for virtual machine allocating."));
    descriptionMemoryLabel -> setWordWrap(true);

    int32_t totalRAM = 0;

    SystemUtils::getTotalMemory(totalRAM);

    spinBoxMemoryLabel = new QLabel("MiB");

    memorySpinBox = new QSpinBox();
    memorySpinBox -> setMinimum(1);
    memorySpinBox -> setMaximum(totalRAM);

    this -> registerField("machine.ram", memorySpinBox, "value", "valueChanged");

    memorySlider = new QSlider(Qt::Horizontal);
    memorySlider -> setTickPosition(QSlider::TicksBelow);
    memorySlider -> setTickInterval(500);
    memorySlider -> setMinimum(1);
    memorySlider -> setMaximum(totalRAM);

    connect(memorySpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            memorySlider, &QSlider::setValue);

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

MachineDiskPage::MachineDiskPage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine virtual hard disk"));

    this -> newMachine = machine;

    machineDiskLabel = new QLabel(
                tr("Select a virtual hard disk to the new machine."
                   "You can either create a new hard disk or select an existing one."));
    machineDiskLabel -> setWordWrap(true);

    machineDiskInfoLabel = new QLabel(tr("If you need a more complex storage set-up"
                                         "you can skip this step and make the changes"
                                         "to the machine settings once the machine is created."));
    machineDiskInfoLabel -> setWordWrap(true);

    machineDiskSizeLabel = new QLabel(tr("The recommended size of the hard disk is"));
    machineDiskSizeLabel -> setWordWrap(true);

    noDiskRadio = new QRadioButton(tr("Do not add a virtual hard disk"));

    createDiskRadio = new QRadioButton(tr("Create a new virtual hard disk"));
    createDiskRadio -> setChecked(true);

    useExistingDiskRadio = new QRadioButton(tr("Use an existing virtual hard disk"));

    connect(useExistingDiskRadio, &QAbstractButton::toggled,
            this, &MachineDiskPage::useExistingDiskToggle);

    hardDiskPathLineEdit = new QLineEdit();
    hardDiskPathLineEdit -> setEnabled(false);

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    this -> useExistingDiskToggle(false);

    useOldDiskLayout = new QHBoxLayout();
    useOldDiskLayout -> setAlignment(Qt::AlignVCenter);
    useOldDiskLayout -> addWidget(hardDiskPathLineEdit);
    useOldDiskLayout -> addWidget(pathNewDiskPushButton);

    machineDiskLayout = new QVBoxLayout();
    machineDiskLayout -> setSpacing(5);
    machineDiskLayout -> addWidget(machineDiskLabel);
    machineDiskLayout -> addWidget(machineDiskInfoLabel);
    machineDiskLayout -> addWidget(machineDiskSizeLabel);
    machineDiskLayout -> addWidget(noDiskRadio);
    machineDiskLayout -> addWidget(createDiskRadio);
    machineDiskLayout -> addWidget(useExistingDiskRadio);
    machineDiskLayout -> addItem(useOldDiskLayout);

    setLayout(machineDiskLayout);

    qDebug() << "MachineDiskPage created";
}

MachineDiskPage::~MachineDiskPage() {
    qDebug() << "MachineDiskPage destroyed";
}

void MachineDiskPage::useExistingDiskToggle(bool toggled) {

    this -> pathNewDiskPushButton -> setEnabled(toggled);
}

MachineNewDiskPage::MachineNewDiskPage(Machine *machine,
                                       QWidget *parent) : QWizardPage(parent) {

    this -> newMachine = machine;

    ////////////////////////////   DISK NAME   ///////////////////////////////////
    fileLocationGroupBox = new QGroupBox(tr("Disk name"));

    fileName = new QLineEdit();

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    fileLocationLayout = new QHBoxLayout();
    fileLocationLayout -> addWidget(fileName);
    fileLocationLayout -> addWidget(pathNewDiskPushButton);

    fileLocationGroupBox -> setLayout(fileLocationLayout);

    //////////////////////////   END DISK NAME   /////////////////////////////////

    ////////////////////////////   DISK SIZE   ///////////////////////////////////
    fileSizeGroupBox = new QGroupBox(tr("Disk size"));

    memoryLabel = new QLabel("GiB");
    diskSpinBox = new QDoubleSpinBox();
    diskSpinBox -> setMinimum(1);
    diskSpinBox -> setMaximum(1000000);
    diskSpinBox -> stepBy(1);

    diskSlider = new QSlider(Qt::Horizontal);
    diskSlider -> setTickPosition(QSlider::TicksBelow);
    diskSlider -> setTickInterval(10);
    diskSlider -> setMinimum(1);
    diskSlider -> setMaximum(100);

    connect(diskSlider, &QSlider::valueChanged,
            diskSpinBox, &QDoubleSpinBox::setValue);

    connect(diskSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            diskSlider, &QSlider::setValue);

    minDiskLabel = new QLabel("1 GiB");
    maxDisklabel = new QLabel("100 GiB");

    fileSizeLayout = new QGridLayout();
    fileSizeLayout -> setColumnStretch(1, 50);

    fileSizeLayout -> addWidget(diskSlider,   1, 0, 1, 3);
    fileSizeLayout -> addWidget(diskSpinBox,  1, 3, 1, 1);
    fileSizeLayout -> addWidget(memoryLabel,  1, 4, 1, 1);
    fileSizeLayout -> addWidget(minDiskLabel, 2, 0, 1, 1);
    fileSizeLayout -> addWidget(maxDisklabel, 2, 2, 1, 1);

    fileSizeGroupBox -> setLayout(fileSizeLayout);

    //////////////////////////   END DISK SIZE   /////////////////////////////////

    ////////////////////////////   DISK TYPE   ///////////////////////////////////
    fileTypeGroupBox = new QGroupBox(tr("Disk type"));

    rawRadioButton   = new QRadioButton(tr("raw"));
    qcowRadioButton  = new QRadioButton(tr("qcow (QEMU Copy-on-write)"));
    qcow2RadioButton = new QRadioButton(tr("qcow2 (QEMU Copy-on-write 2)"));
    qcow2RadioButton -> setChecked(true);
    qedRadioButton   = new QRadioButton(tr("qed (QEMU enhanced disk)"));
    vmdkRadioButton  = new QRadioButton(tr("vmdk (Virtual Machine Disk)"));
    cloopRadioButton = new QRadioButton(tr("cloop (Compressed Loop)"));

    diskTypeLayout = new QGridLayout();

    diskTypeLayout -> addWidget(rawRadioButton,  0, 0, 1, 1);
    diskTypeLayout -> addWidget(qcowRadioButton,  0, 1, 1, 1);
    diskTypeLayout -> addWidget(qcow2RadioButton, 1, 0, 1, 1);
    diskTypeLayout -> addWidget(qedRadioButton,   1, 1, 1, 1);
    diskTypeLayout -> addWidget(vmdkRadioButton,  2, 0, 1, 1);
    diskTypeLayout -> addWidget(cloopRadioButton, 2, 1, 1, 1);

    fileTypeGroupBox -> setLayout(diskTypeLayout);

    //////////////////////////   END DISK TYPE   /////////////////////////////////

    newDiskLayout = new QVBoxLayout();
    newDiskLayout -> addWidget(fileLocationGroupBox);
    newDiskLayout -> addWidget(fileSizeGroupBox);
    newDiskLayout -> addWidget(fileTypeGroupBox);

    setLayout(newDiskLayout);

    qDebug() << "MachineNewDiskPage created";
}

MachineNewDiskPage::~MachineNewDiskPage() {
    qDebug() << "MachineNewDiskPage destroyed";
}

void MachineNewDiskPage::initializePage() {
    fileName -> setText(field("machine.name").toString());
}

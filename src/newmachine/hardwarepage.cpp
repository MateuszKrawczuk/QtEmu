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
#include "hardwarepage.h"

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

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
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
    connect(creativeCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectCreativeCard);

    ensoniqCheck = new QCheckBox("ENSONIQ AudioPCI ES1370");
    connect(ensoniqCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectEnsoniqCard);

    intelAC97Check = new QCheckBox("Intel AC97(82801AA)");
    intelAC97Check -> setChecked(true);
    connect(intelAC97Check, &QAbstractButton::toggled,
                this, &AudioTab::selectIntelAC97Card);

    yamahaCheck = new QCheckBox("Yamaha YM3812");
    connect(yamahaCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectYamahaCard);

    gravisCheck = new QCheckBox("Gravis Ultrasound GF1");
    connect(gravisCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectGravisCard);

    CS4231ACheck = new QCheckBox("CS4231A");
    connect(CS4231ACheck, &QAbstractButton::toggled,
                this, &AudioTab::selectCS4231ACard);

    intelHDACheck = new QCheckBox("Intel HD Audio");
    connect(intelHDACheck, &QAbstractButton::toggled,
                this, &AudioTab::selectIntelHDCard);

    pcSpeakerCheck = new QCheckBox(tr("PC Speaker"));
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
    this -> networkState(true);

    connect(withNetworkRadio, &QAbstractButton::toggled,
                this, &NetworkTab::networkState);

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

void NetworkTab::networkState(bool network) {
    this -> newMachine -> setUseNetwork(network);
}

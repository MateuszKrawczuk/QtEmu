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
#include "hardwarepage.h"

MachineHardwarePage::MachineHardwarePage(Machine *machine,
                                         QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine hardware"));

    this -> m_newMachine = machine;

    m_hardwareTabWidget = new QTabWidget();
    m_hardwareTabWidget -> addTab(new ProcessorTab(machine, Qt::AlignVCenter, this), tr("Processor"));
    m_hardwareTabWidget -> addTab(new GraphicsTab(machine, this), tr("Graphics"));
    m_hardwareTabWidget -> addTab(new AudioTab(machine, this), tr("Audio"));
    m_hardwareTabWidget -> addTab(new NetworkTab(machine, this), tr("Network"));

    m_hardwareLayout = new QVBoxLayout();
    m_hardwareLayout -> setAlignment(Qt::AlignCenter);
    m_hardwareLayout -> addWidget(m_hardwareTabWidget);

    setLayout(m_hardwareLayout);

    qDebug() << "MachineHardwarePage created";
}

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
}

ProcessorTab::ProcessorTab(Machine *machine, Qt::Alignment layoutAlignment,
                           QWidget *parent) : QWidget(parent) {

    this -> m_newMachine = machine;

    m_CPUTypeLabel = new QLabel(tr("CPU Type") + ":");
    m_CPUTypeLabel -> setWordWrap(true);

    m_CPUType = new QComboBox();
    SystemUtils::setCPUTypesx86(m_CPUType);

    this -> selectProcessor(0);

    connect(m_CPUType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProcessorTab::selectProcessor);

    m_CPUTypeLayout = new QHBoxLayout();
    m_CPUTypeLayout -> setAlignment(Qt::AlignVCenter);
    m_CPUTypeLayout -> setSpacing(5);
    m_CPUTypeLayout -> addWidget(m_CPUTypeLabel);
    m_CPUTypeLayout -> addWidget(m_CPUType);

    m_CPUCountLabel = new QLabel(tr("CPU Count") + ":");
    m_CPUCountLabel -> setWordWrap(true);
    m_CPUCountSpinBox = new QSpinBox();
    m_CPUCountSpinBox -> setMinimum(1);
    m_CPUCountSpinBox -> setMaximum(255);

    this -> selectCPUCount(1);
    connect(m_CPUCountSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectCPUCount);

    m_coresSocketLabel = new QLabel(tr("Cores per socket") + ":");
    m_coresSocketLabel -> setWordWrap(true);
    m_coresSocketSpinBox = new QSpinBox();
    m_coresSocketSpinBox -> setMinimum(0);
    m_coresSocketSpinBox -> setMaximum(255);

    connect(m_coresSocketSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectCoresSocket);

    m_CPUCountLayout = new QHBoxLayout();
    m_CPUCountLayout -> setAlignment(Qt::AlignVCenter);
    m_CPUCountLayout -> setSpacing(5);
    m_CPUCountLayout -> addWidget(m_CPUCountLabel);
    m_CPUCountLayout -> addWidget(m_CPUCountSpinBox);
    m_CPUCountLayout -> addWidget(m_coresSocketLabel);
    m_CPUCountLayout -> addWidget(m_coresSocketSpinBox);

    m_socketCountLabel = new QLabel(tr("Socket count") + ":");
    m_socketCountLabel -> setWordWrap(true);
    m_socketCountSpinBox = new QSpinBox();
    m_socketCountSpinBox -> setMinimum(0);
    m_socketCountSpinBox -> setMaximum(255);

    connect(m_socketCountSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectSocketCount);

    m_threadsCoreLabel = new QLabel(tr("Threads per core") + ":");
    m_threadsCoreLabel -> setWordWrap(true);
    m_threadsCoreSpinBox = new QSpinBox();
    m_threadsCoreSpinBox -> setMinimum(0);
    m_threadsCoreSpinBox -> setMaximum(255);

    connect(m_threadsCoreSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectThreadsCore);

    m_socketLayout = new QHBoxLayout();
    m_socketLayout -> setAlignment(Qt::AlignVCenter);
    m_socketLayout -> setSpacing(5);
    m_socketLayout -> addWidget(m_socketCountLabel);
    m_socketLayout -> addWidget(m_socketCountSpinBox);
    m_socketLayout -> addWidget(m_threadsCoreLabel);
    m_socketLayout -> addWidget(m_threadsCoreSpinBox);

    m_maxHotCPULabel = new QLabel(tr("Maximum number of hotpluggable CPUs") + ":");
    m_maxHotCPULabel -> setWordWrap(false);
    m_maxHotCPUSpinBox = new QSpinBox();
    m_maxHotCPUSpinBox -> setMinimum(0);
    m_maxHotCPUSpinBox -> setMaximum(255);

    connect(m_maxHotCPUSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectMaxHotCPU);

    m_maxHotCPUsLayout = new QHBoxLayout();
    m_maxHotCPUsLayout -> setAlignment(Qt::AlignVCenter);
    m_maxHotCPUsLayout -> setSpacing(5);
    m_maxHotCPUsLayout -> addWidget(m_maxHotCPULabel);
    m_maxHotCPUsLayout -> addWidget(m_maxHotCPUSpinBox);

    m_CPUSettingsLayout = new QVBoxLayout();
    m_CPUSettingsLayout -> setAlignment(Qt::AlignVCenter);
    m_CPUSettingsLayout -> addItem(m_CPUCountLayout);
    m_CPUSettingsLayout -> addItem(m_socketLayout);
    m_CPUSettingsLayout -> addItem(m_maxHotCPUsLayout);

    m_CPUSettings = new QGroupBox(tr("CPU Settings"));
    m_CPUSettings -> setLayout(m_CPUSettingsLayout);

    m_processorLayout = new QVBoxLayout();
    m_processorLayout -> setAlignment(layoutAlignment);
    m_processorLayout -> addItem(m_CPUTypeLayout);
    m_processorLayout -> addWidget(m_CPUSettings);

    setLayout(m_processorLayout);

    qDebug() << "ProcessorTab created";
}

ProcessorTab::~ProcessorTab() {
    qDebug() << "ProcessorTab destroyed";
}

void ProcessorTab::selectProcessor(int index) {
    QString cpu = this -> m_CPUType -> itemData(index).toString();

    this -> m_newMachine -> setCPUType(cpu);
}

void ProcessorTab::selectCPUCount(int CPUCount) {
    this -> m_newMachine -> setCPUCount(CPUCount);
}

void ProcessorTab::selectSocketCount(int socketCount) {
    this -> m_newMachine -> setSocketCount(socketCount);
}

void ProcessorTab::selectCoresSocket(int coresSocket) {
    this -> m_newMachine -> setCoresSocket(coresSocket);
}

void ProcessorTab::selectThreadsCore(int threadsCore) {
    this -> m_newMachine -> setThreadsCore(threadsCore);
}

void ProcessorTab::selectMaxHotCPU(int maxCPU) {
    this -> m_newMachine -> setMaxHotCPU(maxCPU);
}

GraphicsTab::GraphicsTab(Machine *machine,
                         QWidget *parent) : QWidget(parent) {

    this -> m_newMachine = machine;

    m_GPUTypeLabel = new QLabel(tr("GPU Type") + ":");
    m_GPUTypeLabel -> setWordWrap(true);
    m_GPUType = new QComboBox();
    SystemUtils::setGPUTypes(m_GPUType);

    m_GPUType -> setCurrentIndex(1);

    this -> selectGraphics(1);

    connect(m_GPUType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GraphicsTab::selectGraphics);

    m_keyboardLabel = new QLabel(tr("Keyboard layout") + ":");
    m_keyboardLabel -> setWordWrap(true);
    m_keyboard = new QComboBox();
    SystemUtils::setKeyboardLayout(m_keyboard);

    m_keyboard -> setCurrentIndex(5);

    this -> selectKeyboard(5);

    connect(m_keyboard, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GraphicsTab::selectKeyboard);

    m_graphicsLayout = new QGridLayout();

    m_graphicsLayout -> setColumnStretch(1, 10);
    m_graphicsLayout -> setColumnStretch(2, 10);

    m_graphicsLayout -> addWidget(m_GPUTypeLabel,  1, 0, 1, 1);
    m_graphicsLayout -> addWidget(m_GPUType,       1, 1, 1, 3);
    m_graphicsLayout -> addWidget(m_keyboardLabel, 2, 0, 1, 1);
    m_graphicsLayout -> addWidget(m_keyboard,      2, 1, 1, 3);

    setLayout(m_graphicsLayout);

    qDebug() << "GraphicsTab created";
}

GraphicsTab::~GraphicsTab() {
    qDebug() << "GraphicsTab destroyed";
}

void GraphicsTab::selectGraphics(int index) {
    QString graphics = this -> m_GPUType -> itemData(index).toString();

    this -> m_newMachine -> setGPUType(graphics);
}

void GraphicsTab::selectKeyboard(int index) {
    QString keyboard = this -> m_keyboard -> itemData(index).toString();

    this -> m_newMachine -> setKeyboard(keyboard);
}

AudioTab::AudioTab(Machine *machine,
                   QWidget *parent) : QWidget(parent) {

    this -> m_newMachine = machine;

    m_creativeCheck = new QCheckBox("Creative Sound Blaster 16");
    connect(m_creativeCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectCreativeCard);

    m_ensoniqCheck = new QCheckBox("ENSONIQ AudioPCI ES1370");
    connect(m_ensoniqCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectEnsoniqCard);

    m_intelAC97Check = new QCheckBox("Intel AC97(82801AA)");
    m_intelAC97Check -> setChecked(true);
    connect(m_intelAC97Check, &QAbstractButton::toggled,
                this, &AudioTab::selectIntelAC97Card);

    m_yamahaCheck = new QCheckBox("Yamaha YM3812");
    connect(m_yamahaCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectYamahaCard);

    m_gravisCheck = new QCheckBox("Gravis Ultrasound GF1");
    connect(m_gravisCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectGravisCard);

    m_CS4231ACheck = new QCheckBox("CS4231A");
    connect(m_CS4231ACheck, &QAbstractButton::toggled,
                this, &AudioTab::selectCS4231ACard);

    m_intelHDACheck = new QCheckBox("Intel HD Audio");
    connect(m_intelHDACheck, &QAbstractButton::toggled,
                this, &AudioTab::selectIntelHDCard);

    m_pcSpeakerCheck = new QCheckBox(tr("PC Speaker"));
    connect(m_pcSpeakerCheck, &QAbstractButton::toggled,
                this, &AudioTab::selectPCSpeakerCard);

    m_audioLayout = new QGridLayout();
    m_audioLayout -> addWidget(m_creativeCheck,  0, 0, 1, 1);
    m_audioLayout -> addWidget(m_ensoniqCheck,   0, 1, 1, 1);

    m_audioLayout -> addWidget(m_intelAC97Check, 1, 0, 1, 1);
    m_audioLayout -> addWidget(m_yamahaCheck,    1, 1, 1, 1);

    m_audioLayout -> addWidget(m_gravisCheck,    2, 0, 1, 1);
    m_audioLayout -> addWidget(m_CS4231ACheck,   2, 1, 1, 1);

    m_audioLayout -> addWidget(m_intelHDACheck,  3, 0, 1, 1);
    m_audioLayout -> addWidget(m_pcSpeakerCheck, 3, 1, 1, 1);

    setLayout(m_audioLayout);

    qDebug() << "AudioTab created";
}

AudioTab::~AudioTab() {
    qDebug() << "AudioTab destroyed";
}

void AudioTab::selectCreativeCard(bool selectCreative) {
    if (selectCreative) {
        this -> m_newMachine -> addAudio("sb16");
    } else {
        this -> m_newMachine -> removeAudio("sb16");
    }
}

void AudioTab::selectEnsoniqCard(bool selectEnsoniq) {
    if (selectEnsoniq) {
        this -> m_newMachine -> addAudio("es1370");
    } else {
        this -> m_newMachine -> removeAudio("es1370");
    }
}

void AudioTab::selectIntelAC97Card(bool selectIntelAC97) {
    if (selectIntelAC97) {
        this -> m_newMachine -> addAudio("ac97");
    } else {
        this -> m_newMachine -> removeAudio("ac97");
    }
}

void AudioTab::selectYamahaCard(bool selectYamaha) {
    if (selectYamaha) {
        this -> m_newMachine -> addAudio("adlib");
    } else {
        this -> m_newMachine -> removeAudio("adlib");
    }
}

void AudioTab::selectGravisCard(bool selectGravis) {
    if (selectGravis) {
        this -> m_newMachine -> addAudio("gus");
    } else {
        this -> m_newMachine -> removeAudio("gus");
    }
}

void AudioTab::selectCS4231ACard(bool selectCS4231A) {
    if (selectCS4231A) {
        this -> m_newMachine -> addAudio("cs4231a");
    } else {
        this -> m_newMachine -> removeAudio("cs4231a");
    }
}

void AudioTab::selectIntelHDCard(bool selectIntelHD) {
    if (selectIntelHD) {
        this -> m_newMachine -> addAudio("hda");
    } else {
        this -> m_newMachine -> removeAudio("hda");
    }
}

void AudioTab::selectPCSpeakerCard(bool selectPCSpeaker) {
    if (selectPCSpeaker) {
        this -> m_newMachine -> addAudio("pcspk");
    } else {
        this -> m_newMachine -> removeAudio("pcspk");
    }
}

NetworkTab::NetworkTab(Machine *machine,
                       QWidget *parent) : QWidget(parent) {

    this -> m_newMachine = machine;

    m_withNetworkRadio = new QRadioButton(tr("User Mode Network Connection (Uses the user mode network stack)"));
    m_withNetworkRadio -> setChecked(true);
    this -> networkState(true);

    connect(m_withNetworkRadio, &QAbstractButton::toggled,
                this, &NetworkTab::networkState);

    m_withoutNetworkRadio = new QRadioButton(tr("No network (No network cards installed on this machine"));

    m_networkLayout = new QVBoxLayout();
    m_networkLayout -> addWidget(m_withNetworkRadio);
    m_networkLayout -> addWidget(m_withoutNetworkRadio);

    setLayout(m_networkLayout);

    qDebug() << "NetworkTab created";
}

NetworkTab::~NetworkTab() {
    qDebug() << "NetworkTab destroyed";
}

void NetworkTab::networkState(bool network) {
    this -> m_newMachine -> setUseNetwork(network);
}

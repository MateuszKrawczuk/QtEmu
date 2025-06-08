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
#include "hardwarepage.h"
#include <thread>

/**
 * @brief Machine hardware page
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Hardware page section. In this page you can select the cpu, graphics, audio and
 * network for the new machine
 */
MachineHardwarePage::MachineHardwarePage(Machine *machine,
                                         QWidget *parent) : QWizardPage(parent) {
    this->setTitle(tr("Machine hardware"));
    this->m_newMachine = machine;

    m_hardwareTabWidget = new QTabWidget(this);
    m_hardwareTabWidget->addTab(new ProcessorTab(machine, this), tr("Processor"));
    m_hardwareTabWidget->addTab(new GraphicsTab(machine, this), tr("Graphics"));
    m_hardwareTabWidget->addTab(new AudioTab(machine, this), tr("Audio"));
    m_hardwareTabWidget->addTab(new NetworkTab(machine, this), tr("Network"));

    m_hardwareLayout = new QVBoxLayout();
    m_hardwareLayout->setAlignment(Qt::AlignCenter);
    m_hardwareLayout->addWidget(m_hardwareTabWidget);

    this->setLayout(m_hardwareLayout);

    qDebug() << "MachineHardwarePage created";
}

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
}

/**
 * @brief Processor tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Processor tab. In this tab you can select the CPU type, etc...
 */
ProcessorTab::ProcessorTab(Machine *machine,
                           QWidget *parent) : QWidget(parent) {
    this->m_newMachine = machine;

    m_CPUTypeLabel = new QLabel(tr("CPU Type") + ":", this);
    m_CPUTypeLabel->setWordWrap(true);

    m_CPUType = new QComboBox(this);
    SystemUtils::setCPUTypesx86(m_CPUType);
    auto core_count = std::thread::hardware_concurrency();

    m_CPUType->setCurrentText("Max");
    this->selectProcessor(m_CPUType->currentIndex());

    connect(m_CPUType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProcessorTab::selectProcessor);

    m_CPUTypeLayout = new QHBoxLayout();
    m_CPUTypeLayout->setAlignment(Qt::AlignVCenter);
    m_CPUTypeLayout->setSpacing(5);
    m_CPUTypeLayout->addWidget(m_CPUTypeLabel);
    m_CPUTypeLayout->addWidget(m_CPUType);

    m_CPUCountLabel = new QLabel(tr("SMP Core Count") + ":", this);
    m_CPUCountLabel->setWordWrap(true);

    // Create and configure CPU count spinbox
    m_CPUCountSpinBox = new QSpinBox(this);
    m_CPUCountSpinBox->setMinimum(1);
    m_CPUCountSpinBox->setMaximum(core_count);

    // Create and configure CPU count slider
    m_CPUCountSlider = new QSlider(Qt::Horizontal, this);
    m_CPUCountSlider->setTickPosition(QSlider::TicksBelow);
    m_CPUCountSlider->setTickInterval(1);
    m_CPUCountSlider->setMinimum(1);
    m_CPUCountSlider->setMaximum(core_count);

    // Set initial values
    if (core_count >= 3) {
        m_CPUCountSpinBox->setValue(core_count / 2);
        m_CPUCountSlider->setValue(core_count / 2);
    } else {
        m_CPUCountSpinBox->setValue(1);
        m_CPUCountSlider->setValue(1);
    }

    // Connect slider and spinbox for synchronization
    connect(m_CPUCountSlider, &QSlider::valueChanged,
            m_CPUCountSpinBox, &QSpinBox::setValue);
    connect(m_CPUCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_CPUCountSlider, &QSlider::setValue);
    connect(m_CPUCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ProcessorTab::selectCPUCount);

    // Update CPU count in machine
    this->selectCPUCount(m_CPUCountSpinBox->value());

    // Create min/max CPU labels
    m_minCPULabel = new QLabel("1 Core", this);
    m_maxCPULabel = new QLabel(QString::number(core_count) + " Cores", this);

    // Update layout to include slider and labels with spinbox on the right
    m_CPUCountLayout = new QVBoxLayout();
    m_CPUCountLayout->setAlignment(Qt::AlignVCenter);
    m_CPUCountLayout->setSpacing(5);
    m_CPUCountLayout->addWidget(m_CPUCountLabel);

    // Create horizontal layout for slider and spinbox
    QHBoxLayout *sliderSpinLayout = new QHBoxLayout();
    sliderSpinLayout->addWidget(m_CPUCountSlider);
    sliderSpinLayout->addWidget(m_CPUCountSpinBox);
    m_CPUCountLayout->addLayout(sliderSpinLayout);

    // Add min/max labels layout
    QHBoxLayout *cpuLabelsLayout = new QHBoxLayout();
    cpuLabelsLayout->addWidget(m_minCPULabel);
    cpuLabelsLayout->addStretch();
    cpuLabelsLayout->addWidget(m_maxCPULabel);
    m_CPUCountLayout->addLayout(cpuLabelsLayout);

    m_CPUSettingsLayout = new QVBoxLayout();
    m_CPUSettingsLayout->setAlignment(Qt::AlignVCenter);
    m_CPUSettingsLayout->addItem(m_CPUCountLayout);


    m_CPUSettings = new QGroupBox(tr("CPU Settings"), this);
    m_CPUSettings->setLayout(m_CPUSettingsLayout);


    m_processorLayout = new QVBoxLayout();
    m_processorLayout->setAlignment(Qt::AlignVCenter);
    m_processorLayout->addItem(m_CPUTypeLayout);
    m_processorLayout->addWidget(m_CPUSettings);

    this->setLayout(m_processorLayout);

    qDebug() << "ProcessorTab created";
}

ProcessorTab::~ProcessorTab() {
    qDebug() << "ProcessorTab destroyed";
}


void ProcessorTab::selectProcessor(int index) {
    QString cpu = this->m_CPUType->itemData(index).toString();
    this->m_newMachine->setCPUType(cpu);
}

/**
 * @brief Select the processor count
 * @param CPUCount, number of processor count
 *
 * Select the processor count
 */
void ProcessorTab::selectCPUCount(int CPUCount) {
    qDebug() << "Setting CPU count to:" << CPUCount;
    this->m_newMachine->setCPUCount(CPUCount);
}

/**
 * @brief GraphicsTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * GraphicsTab tab. In this tab you can select the GPU type and
 * keyboard
 */
GraphicsTab::GraphicsTab(Machine *machine,
                         QWidget *parent) : QWidget(parent) {
    this->m_newMachine = machine;

    m_GPUTypeLabel = new QLabel(tr("GPU Type") + ":", this);
    m_GPUTypeLabel->setWordWrap(true);
    m_GPUType = new QComboBox(this);
    SystemUtils::setGPUTypes(m_GPUType);
    m_GPUType->setCurrentIndex(1);
    this->selectGraphics(1);

    connect(m_GPUType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GraphicsTab::selectGraphics);

    m_keyboardLabel = new QLabel(tr("Keyboard layout") + ":", this);
    m_keyboardLabel->setWordWrap(true);
    m_keyboard = new QComboBox(this);
    SystemUtils::setKeyboardLayout(m_keyboard);
    m_keyboard->setCurrentIndex(5);
    this->selectKeyboard(5);

    connect(m_keyboard, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GraphicsTab::selectKeyboard);

    m_graphicsLayout = new QGridLayout();
    m_graphicsLayout->setColumnStretch(1, 10);
    m_graphicsLayout->setColumnStretch(2, 10);
    m_graphicsLayout->addWidget(m_GPUTypeLabel, 1, 0, 1, 1);
    m_graphicsLayout->addWidget(m_GPUType, 1, 1, 1, 3);
    m_graphicsLayout->addWidget(m_keyboardLabel, 2, 0, 1, 1);
    m_graphicsLayout->addWidget(m_keyboard, 2, 1, 1, 3);

    this->setLayout(m_graphicsLayout);

    qDebug() << "GraphicsTab created";
}

GraphicsTab::~GraphicsTab() {
    qDebug() << "GraphicsTab destroyed";
}

/**
 * @brief Select the GPU type
 * @param index, index of the gpu combo
 *
 * Select the GPU type
 */
void GraphicsTab::selectGraphics(int index) {
    QString graphics = this->m_GPUType->itemData(index).toString();
    this->m_newMachine->setGPUType(graphics);
}

/**
 * @brief Select the keyboard layout
 * @param index, index of the keyboard combo
 *
 * Select the keyboard layout
 */
void GraphicsTab::selectKeyboard(int index) {
    QString keyboard = this->m_keyboard->itemData(index).toString();
    this->m_newMachine->setKeyboard(keyboard);
}

/**
 * @brief AudioTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * AudioTab tab. In this tab you can select the audio cards
 * for the machine
 */
AudioTab::AudioTab(Machine *machine,
                   QWidget *parent) : QWidget(parent) {
    this->m_newMachine = machine;

    m_creativeCheck = new QCheckBox("Creative Sound Blaster 16", this);
    connect(m_creativeCheck, &QAbstractButton::toggled,
            this, &AudioTab::selectCreativeCard);

    m_ensoniqCheck = new QCheckBox("ENSONIQ AudioPCI ES1370", this);
    connect(m_ensoniqCheck, &QAbstractButton::toggled,
            this, &AudioTab::selectEnsoniqCard);

    m_intelAC97Check = new QCheckBox("Intel AC97(82801AA)", this);
    m_intelAC97Check->setChecked(true);
    connect(m_intelAC97Check, &QAbstractButton::toggled,
            this, &AudioTab::selectIntelAC97Card);

    m_yamahaCheck = new QCheckBox("Yamaha YM3812", this);
    connect(m_yamahaCheck, &QAbstractButton::toggled,
            this, &AudioTab::selectYamahaCard);

    m_gravisCheck = new QCheckBox("Gravis Ultrasound GF1", this);
    connect(m_gravisCheck, &QAbstractButton::toggled,
            this, &AudioTab::selectGravisCard);

    m_CS4231ACheck = new QCheckBox("CS4231A", this);
    connect(m_CS4231ACheck, &QAbstractButton::toggled,
            this, &AudioTab::selectCS4231ACard);

    m_intelHDACheck = new QCheckBox("Intel HD Audio", this);
    connect(m_intelHDACheck, &QAbstractButton::toggled,
            this, &AudioTab::selectIntelHDCard);

    m_pcSpeakerCheck = new QCheckBox(tr("PC Speaker"), this);
    connect(m_pcSpeakerCheck, &QAbstractButton::toggled,
            this, &AudioTab::selectPCSpeakerCard);

    m_audioLayout = new QGridLayout();
    m_audioLayout->addWidget(m_creativeCheck, 0, 0, 1, 1);
    m_audioLayout->addWidget(m_ensoniqCheck, 0, 1, 1, 1);
    m_audioLayout->addWidget(m_intelAC97Check, 1, 0, 1, 1);
    m_audioLayout->addWidget(m_yamahaCheck, 1, 1, 1, 1);
    m_audioLayout->addWidget(m_gravisCheck, 2, 0, 1, 1);
    m_audioLayout->addWidget(m_CS4231ACheck, 2, 1, 1, 1);
    m_audioLayout->addWidget(m_intelHDACheck, 3, 0, 1, 1);
    m_audioLayout->addWidget(m_pcSpeakerCheck, 3, 1, 1, 1);

    this->setLayout(m_audioLayout);

    qDebug() << "AudioTab created";
}

AudioTab::~AudioTab() {
    qDebug() << "AudioTab destroyed";
}

/**
 * @brief Add or remove the creative audio card
 * @param selectCreative, true add the card, false remove it
 *
 * Add or remove the creative audio card
 */
void AudioTab::selectCreativeCard(bool selectCreative) {
    if (selectCreative) {
        this->m_newMachine->addAudio("sb16");
    } else {
        this->m_newMachine->removeAudio("sb16");
    }
}

/**
 * @brief Add or remove the Ensoniq audio card
 * @param selectEnsoniq, true add the card, false remove it
 *
 * Add or remove the Ensoniq audio card
 */
void AudioTab::selectEnsoniqCard(bool selectEnsoniq) {
    if (selectEnsoniq) {
        this->m_newMachine->addAudio("es1370");
    } else {
        this->m_newMachine->removeAudio("es1370");
    }
}

/**
 * @brief Add or remove the IntelAC97 audio card
 * @param selectIntelAC97, true add the card, false remove it
 *
 * Add or remove the IntelAC97 audio card
 */
void AudioTab::selectIntelAC97Card(bool selectIntelAC97) {
    if (selectIntelAC97) {
        this->m_newMachine->addAudio("ac97");
    } else {
        this->m_newMachine->removeAudio("ac97");
    }
}

/**
 * @brief Add or remove the Yamaha audio card
 * @param selectYamaha, true add the card, false remove it
 *
 * Add or remove the Yamaha audio card
 */
void AudioTab::selectYamahaCard(bool selectYamaha) {
    if (selectYamaha) {
        this->m_newMachine->addAudio("adlib");
    } else {
        this->m_newMachine->removeAudio("adlib");
    }
}

/**
 * @brief Add or remove the Gravis audio card
 * @param selectGravis, true add the card, false remove it
 *
 * Add or remove the Gravis audio card
 */
void AudioTab::selectGravisCard(bool selectGravis) {
    if (selectGravis) {
        this->m_newMachine->addAudio("gus");
    } else {
        this->m_newMachine->removeAudio("gus");
    }
}

/**
 * @brief Add or remove the CS4231A audio card
 * @param selectCS4231A, true add the card, false remove it
 *
 * Add or remove the CS4231A audio card
 */
void AudioTab::selectCS4231ACard(bool selectCS4231A) {
    if (selectCS4231A) {
        this->m_newMachine->addAudio("cs4231a");
    } else {
        this->m_newMachine->removeAudio("cs4231a");
    }
}

/**
 * @brief Add or remove the IntelHD audio card
 * @param selectIntelHD, true add the card, false remove it
 *
 * Add or remove the IntelHD audio card
 */
void AudioTab::selectIntelHDCard(bool selectIntelHD) {
    if (selectIntelHD) {
        this->m_newMachine->addAudio("intel-hda");
        this->m_newMachine->addAudio("hda-duplex");
    } else {
        this->m_newMachine->addAudio("intel-hda");
        this->m_newMachine->removeAudio("hda-duplex");
    }
}

/**
 * @brief Add or remove the PC Speaker audio card
 * @param selectPCSpeaker, true add the card, false remove it
 *
 * Add or remove the PC Speaker audio card
 */
void AudioTab::selectPCSpeakerCard(bool selectPCSpeaker) {
    if (selectPCSpeaker) {
        this->m_newMachine->addAudio("pcspk");
    } else {
        this->m_newMachine->removeAudio("pcspk");
    }
}

/**
 * @brief NetworkTab tab
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * NetworkTab tab. In this tab you can enable or disable
 * the network
 */
NetworkTab::NetworkTab(Machine *machine,
                       QWidget *parent) : QWidget(parent) {
    this->m_newMachine = machine;

    m_withNetworkRadio = new QRadioButton(tr("User Mode Network Connection (Uses the user mode network stack)"), this);
    m_withNetworkRadio->setChecked(true);
    this->networkState(true);

    connect(m_withNetworkRadio, &QAbstractButton::toggled,
            this, &NetworkTab::networkState);

    m_withoutNetworkRadio = new QRadioButton(tr("No network (No network cards installed on this machine"), this);

    m_networkLayout = new QVBoxLayout();
    m_networkLayout->addWidget(m_withNetworkRadio);
    m_networkLayout->addWidget(m_withoutNetworkRadio);

    this->setLayout(m_networkLayout);

    qDebug() << "NetworkTab created";
}

NetworkTab::~NetworkTab() {
    qDebug() << "NetworkTab destroyed";
}

/**
 * @brief Set the network state
 * @param network, establish the network use
 *
 * Enable or disable the network in the machine
 */
void NetworkTab::networkState(bool network) {
    this->m_newMachine->setUseNetwork(network);
}

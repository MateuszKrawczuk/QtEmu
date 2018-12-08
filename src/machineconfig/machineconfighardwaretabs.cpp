/*
 * This file is part of QtEmu project.
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
#include "machineconfighardwaretabs.h"

/**
 * @brief Tab with informacion about the machine CPU options
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * Tab with informacion about the machine CPU options
 */
ProcessorConfigTab::ProcessorConfigTab(Machine *machine,
                                       QWidget *parent) : QWidget(parent)
{
    m_CPUTypeLabel = new QLabel(tr("CPU Type") + ":", this);
    m_CPUTypeLabel->setWordWrap(true);

    m_CPUType = new QComboBox(this);
    SystemUtils::setCPUTypesx86(m_CPUType);
    int cpuTypeIndex = m_CPUType->findData(machine->getCPUType());
    if (cpuTypeIndex != -1) {
       m_CPUType->setCurrentIndex(cpuTypeIndex);
    }

    m_CPUTypeLayout = new QHBoxLayout();
    m_CPUTypeLayout->setAlignment(Qt::AlignVCenter);
    m_CPUTypeLayout->setSpacing(5);
    m_CPUTypeLayout->addWidget(m_CPUTypeLabel);
    m_CPUTypeLayout->addWidget(m_CPUType);

    m_CPUCountLabel = new QLabel(tr("CPU Count") + ":", this);
    m_CPUCountLabel->setWordWrap(true);

    m_CPUCountSpinBox = new QSpinBox(this);
    m_CPUCountSpinBox->setMinimum(1);
    m_CPUCountSpinBox->setMaximum(255);
    m_CPUCountSpinBox->setValue(machine->getCPUCount());

    m_coresSocketLabel = new QLabel(tr("Cores per socket") + ":", this);
    m_coresSocketLabel->setWordWrap(true);

    m_coresSocketSpinBox = new QSpinBox(this);
    m_coresSocketSpinBox->setMinimum(0);
    m_coresSocketSpinBox->setMaximum(255);
    m_coresSocketSpinBox->setValue(machine->getCoresSocket());

    m_CPUCountLayout = new QHBoxLayout();
    m_CPUCountLayout->setAlignment(Qt::AlignVCenter);
    m_CPUCountLayout->setSpacing(5);
    m_CPUCountLayout->addWidget(m_CPUCountLabel);
    m_CPUCountLayout->addWidget(m_CPUCountSpinBox);
    m_CPUCountLayout->addWidget(m_coresSocketLabel);
    m_CPUCountLayout->addWidget(m_coresSocketSpinBox);

    m_socketCountLabel = new QLabel(tr("Socket count") + ":", this);
    m_socketCountLabel->setWordWrap(true);

    m_socketCountSpinBox = new QSpinBox(this);
    m_socketCountSpinBox->setMinimum(0);
    m_socketCountSpinBox->setMaximum(255);
    m_socketCountSpinBox->setValue(machine->getSocketCount());

    m_threadsCoreLabel = new QLabel(tr("Threads per core") + ":", this);
    m_threadsCoreLabel->setWordWrap(true);

    m_threadsCoreSpinBox = new QSpinBox(this);
    m_threadsCoreSpinBox->setMinimum(0);
    m_threadsCoreSpinBox->setMaximum(255);
    m_threadsCoreSpinBox->setValue(machine->getThreadsCore());

    m_socketLayout = new QHBoxLayout();
    m_socketLayout->setAlignment(Qt::AlignVCenter);
    m_socketLayout->setSpacing(5);
    m_socketLayout->addWidget(m_socketCountLabel);
    m_socketLayout->addWidget(m_socketCountSpinBox);
    m_socketLayout->addWidget(m_threadsCoreLabel);
    m_socketLayout->addWidget(m_threadsCoreSpinBox);

    m_maxHotCPULabel = new QLabel(tr("Maximum number of hotpluggable CPUs") + ":", this);
    m_maxHotCPULabel->setWordWrap(false);

    m_maxHotCPUSpinBox = new QSpinBox(this);
    m_maxHotCPUSpinBox->setMinimum(0);
    m_maxHotCPUSpinBox->setMaximum(255);
    m_maxHotCPUSpinBox->setValue(machine->getMaxHotCPU());

    m_maxHotCPUsLayout = new QHBoxLayout();
    m_maxHotCPUsLayout->setAlignment(Qt::AlignVCenter);
    m_maxHotCPUsLayout->setSpacing(5);
    m_maxHotCPUsLayout->addWidget(m_maxHotCPULabel);
    m_maxHotCPUsLayout->addWidget(m_maxHotCPUSpinBox);

    m_CPUSettingsLayout = new QVBoxLayout();
    m_CPUSettingsLayout->setAlignment(Qt::AlignVCenter);
    m_CPUSettingsLayout->addItem(m_CPUCountLayout);
    m_CPUSettingsLayout->addItem(m_socketLayout);
    m_CPUSettingsLayout->addItem(m_maxHotCPUsLayout);

    m_CPUSettings = new QGroupBox(tr("CPU Settings"), this);
    m_CPUSettings->setLayout(m_CPUSettingsLayout);

    m_processorLayout = new QVBoxLayout();
    m_processorLayout->setAlignment(Qt::AlignTop);
    m_processorLayout->addItem(m_CPUTypeLayout);
    m_processorLayout->addWidget(m_CPUSettings);

    this->setLayout(m_processorLayout);

    qDebug() << "ProcessorConfigTab created";
}

ProcessorConfigTab::~ProcessorConfigTab()
{
    qDebug() << "ProcessorConfigTab destroyed";
}

/**
 * @brief Get the CPU type
 * @return CPU type
 *
 * Get the CPU type
 */
QString ProcessorConfigTab::getCPUType()
{
    return this->m_CPUType->currentData().toString();
}

/**
 * @brief Get the CPU count
 * @return CPU count
 *
 * Get the CPU count
 */
int ProcessorConfigTab::getCPUCount()
{
    return this->m_CPUCountSpinBox->value();
}

/**
 * @brief Get the cores per socket
 * @return cores per socket
 *
 * Get the cores per socket
 */
int ProcessorConfigTab::getCoresSocket()
{
    return this->m_coresSocketSpinBox->value();
}

/**
 * @brief Get the socket count
 * @return socket count
 *
 * Get the socket count
 */
int ProcessorConfigTab::getSocketCount()
{
    return this->m_socketCountSpinBox->value();
}

/**
 * @brief Get number of threads per core
 * @return threads per core
 *
 * Get number of threads per core
 */
int ProcessorConfigTab::getThreadsCore()
{
    return this->m_threadsCoreSpinBox->value();
}

/**
 * @brief Get the maximum number of hotpluggable CPUs
 * @return maximum number of hotpluggable CPUs
 *
 * Get the maximum number of hotpluggable CPUs
 */
int ProcessorConfigTab::getMaxHotCPU()
{
    return this->m_maxHotCPUSpinBox->value();
}

/**
 * @brief Tab with the GPU and keyboard
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * Tab with the GPU and keyboard
 */
GraphicsConfigTab::GraphicsConfigTab(Machine *machine,
                                     QWidget *parent) : QWidget(parent)
{
    m_GPUTypeLabel = new QLabel(tr("GPU Type") + ":", this);
    m_GPUTypeLabel->setWordWrap(true);
    m_GPUType = new QComboBox(this);
    SystemUtils::setGPUTypes(m_GPUType);
    int gpuIndex = m_GPUType->findData(machine->getGPUType());
    if (gpuIndex != -1) {
       m_GPUType->setCurrentIndex(gpuIndex);
    }

    m_keyboardLabel = new QLabel(tr("Keyboard layout") + ":", this);
    m_keyboardLabel->setWordWrap(true);
    m_keyboard = new QComboBox(this);
    SystemUtils::setKeyboardLayout(m_keyboard);
    int keyboardIndex = m_keyboard->findData(machine->getKeyboard());
    if (keyboardIndex != -1) {
        m_keyboard->setCurrentIndex(keyboardIndex);
    }

    m_graphicsLayout = new QGridLayout();
    m_graphicsLayout->setColumnStretch(1, 10);
    m_graphicsLayout->setColumnStretch(2, 10);
    m_graphicsLayout->addWidget(m_GPUTypeLabel,  1, 0, 1, 1);
    m_graphicsLayout->addWidget(m_GPUType,       1, 1, 1, 3);
    m_graphicsLayout->addWidget(m_keyboardLabel, 2, 0, 1, 1);
    m_graphicsLayout->addWidget(m_keyboard,      2, 1, 1, 3);

    this->setLayout(m_graphicsLayout);

    qDebug() << "GraphicsConfigTab created";
}

GraphicsConfigTab::~GraphicsConfigTab()
{
    qDebug() << "GraphicsConfigTab destroyed";
}

/**
 * @brief Get the GPU type
 * @return GPU type
 *
 * Get the GPU type
 */
QString GraphicsConfigTab::getGPUType()
{
    return this->m_GPUType->currentData().toString();
}

/**
 * @brief Get the keyboard layout
 * @return keyboard layout
 *
 * Get the keyboard layout
 */
QString GraphicsConfigTab::getKeyboardLayout()
{
    return this->m_keyboard->currentData().toString();
}

/**
 * @brief Tab with the amount of RAM
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * Tab with the amount of RAM
 */
RamConfigTab::RamConfigTab(Machine *machine,
                           QWidget *parent) : QWidget(parent)
{
    m_descriptionMemoryLabel = new QLabel(tr("Select the amount of base memory (RAM) in "
                                             "megabytes for virtual machine allocating."),
                                          this);
    m_descriptionMemoryLabel->setWordWrap(true);

    int totalRAM = 0;
    SystemUtils::getTotalMemory(totalRAM);
    m_spinBoxMemoryLabel = new QLabel("MiB", this);

    m_memorySpinBox = new QSpinBox(this);
    m_memorySpinBox->setMinimum(1);
    m_memorySpinBox->setMaximum(totalRAM);
    m_memorySpinBox->setValue(static_cast<int>(machine->getRAM()));

    m_memorySlider = new QSlider(Qt::Horizontal, this);
    m_memorySlider->setTickPosition(QSlider::TicksBelow);
    m_memorySlider->setTickInterval(500);
    m_memorySlider->setMinimum(1);
    m_memorySlider->setMaximum(totalRAM);
    m_memorySlider->setValue(static_cast<int>(machine->getRAM()));

    connect(m_memorySpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            m_memorySlider, &QSlider::setValue);

    connect(m_memorySlider, &QSlider::valueChanged,
            m_memorySpinBox, &QSpinBox::setValue);

    m_minMemoryLabel = new QLabel("1 MiB", this);
    m_maxMemorylabel = new QLabel(QString("%1 MiB").arg(totalRAM), this);

    m_machineMemoryLayout = new QGridLayout();
    m_machineMemoryLayout->setColumnStretch(1, 50);
    m_machineMemoryLayout->addWidget(m_descriptionMemoryLabel, 0, 0, 1, 5);
    m_machineMemoryLayout->addWidget(m_memorySlider,           1, 0, 1, 3);
    m_machineMemoryLayout->addWidget(m_memorySpinBox,          1, 3, 1, 1);
    m_machineMemoryLayout->addWidget(m_spinBoxMemoryLabel,     1, 4, 1, 1);
    m_machineMemoryLayout->addWidget(m_minMemoryLabel,         2, 0, 1, 1);
    m_machineMemoryLayout->addWidget(m_maxMemorylabel,         2, 2, 1, 1);

    this->setLayout(m_machineMemoryLayout);

    qDebug() << "RamConfigTab created";
}

RamConfigTab::~RamConfigTab()
{
    qDebug() << "RamConfigTab destroyed";
}

/**
 * @brief Get the ram selected
 * @return ram selected
 *
 * Get the ram selected
 */
int RamConfigTab::getAmountRam()
{
    return this->m_memorySpinBox->value();
}

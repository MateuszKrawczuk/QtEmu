/*
 * This file is part of QtEmu project.
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
#include "machineconfighardwaretabs.h"

/**
 * @brief Tab with informacion about the machine CPU options
 * @param machine, machine to be configured
 * @param enableFields, fields enabled or disabled
 * @param parent, parent widget
 *
 * Tab with informacion about the machine CPU options
 */
ProcessorConfigTab::ProcessorConfigTab(Machine *machine,
                                       bool enableFields,
                                       QWidget *parent) : QWidget(parent)
{
    m_CPUTypeLabel = new QLabel(tr("CPU Type") + ":", this);
    m_CPUTypeLabel->setWordWrap(true);

    m_CPUType = new QComboBox(this);
    m_CPUType->setEnabled(enableFields);
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
    m_CPUCountSpinBox->setEnabled(enableFields);

    m_coresSocketLabel = new QLabel(tr("Cores per socket") + ":", this);
    m_coresSocketLabel->setWordWrap(true);

    m_coresSocketSpinBox = new QSpinBox(this);
    m_coresSocketSpinBox->setMinimum(0);
    m_coresSocketSpinBox->setMaximum(255);
    m_coresSocketSpinBox->setValue(machine->getCoresSocket());
    m_coresSocketSpinBox->setEnabled(enableFields);

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
    m_socketCountSpinBox->setEnabled(enableFields);

    m_threadsCoreLabel = new QLabel(tr("Threads per core") + ":", this);
    m_threadsCoreLabel->setWordWrap(true);

    m_threadsCoreSpinBox = new QSpinBox(this);
    m_threadsCoreSpinBox->setMinimum(0);
    m_threadsCoreSpinBox->setMaximum(255);
    m_threadsCoreSpinBox->setValue(machine->getThreadsCore());
    m_threadsCoreSpinBox->setEnabled(enableFields);

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
    m_maxHotCPUSpinBox->setEnabled(enableFields);

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
 * @param enableFields, fields enabled or disabled
 * @param parent, parent widget
 *
 * Tab with the GPU and keyboard
 */
GraphicsConfigTab::GraphicsConfigTab(Machine *machine,
                                     bool enableFields,
                                     QWidget *parent) : QWidget(parent)
{
    m_GPUTypeLabel = new QLabel(tr("GPU Type") + ":", this);
    m_GPUTypeLabel->setWordWrap(true);
    m_GPUType = new QComboBox(this);
    m_GPUType->setEnabled(enableFields);
    SystemUtils::setGPUTypes(m_GPUType);
    int gpuIndex = m_GPUType->findData(machine->getGPUType());
    if (gpuIndex != -1) {
       m_GPUType->setCurrentIndex(gpuIndex);
    }

    m_gpuLayout = new QHBoxLayout();
    m_gpuLayout->addWidget(m_GPUTypeLabel);
    m_gpuLayout->addWidget(m_GPUType);

    m_keyboardLabel = new QLabel(tr("Keyboard layout") + ":", this);
    m_keyboardLabel->setWordWrap(true);
    m_keyboard = new QComboBox(this);
    m_keyboard->setEnabled(enableFields);
    SystemUtils::setKeyboardLayout(m_keyboard);
    int keyboardIndex = m_keyboard->findData(machine->getKeyboard());
    if (keyboardIndex != -1) {
        m_keyboard->setCurrentIndex(keyboardIndex);
    }

    m_keyboardLayout = new QHBoxLayout();
    m_keyboardLayout->addWidget(m_keyboardLabel);
    m_keyboardLayout->addWidget(m_keyboard);

    m_graphicsLayout = new QVBoxLayout();
    m_graphicsLayout->setAlignment(Qt::AlignTop);
    m_graphicsLayout->addItem(m_gpuLayout);
    m_graphicsLayout->addItem(m_keyboardLayout);

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
 * @param enableFields, fields enabled or disabled
 * @param parent, parent widget
 *
 * Tab with the amount of RAM
 */
RamConfigTab::RamConfigTab(Machine *machine,
                           bool enableFields,
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
    m_memorySpinBox->setEnabled(enableFields);

    m_memorySlider = new QSlider(Qt::Horizontal, this);
    m_memorySlider->setTickPosition(QSlider::TicksBelow);
    m_memorySlider->setTickInterval(500);
    m_memorySlider->setMinimum(1);
    m_memorySlider->setMaximum(totalRAM);
    m_memorySlider->setValue(static_cast<int>(machine->getRAM()));
    m_memorySlider->setEnabled(enableFields);

    connect(m_memorySpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            m_memorySlider, &QSlider::setValue);

    connect(m_memorySlider, &QSlider::valueChanged,
            m_memorySpinBox, &QSpinBox::setValue);

    m_minMemoryLabel = new QLabel("1 MiB", this);
    m_maxMemorylabel = new QLabel(QString("%1 MiB").arg(totalRAM), this);

    m_machineMemoryLayout = new QGridLayout();
    m_machineMemoryLayout->setRowStretch(1, 1);
    m_machineMemoryLayout->setRowStretch(2, 10);
    m_machineMemoryLayout->setColumnStretch(1, 10);
    m_machineMemoryLayout->addWidget(m_descriptionMemoryLabel, 0, 0, 1, 5, Qt::AlignTop);
    m_machineMemoryLayout->addWidget(m_memorySlider,           1, 0, 1, 3, Qt::AlignTop);
    m_machineMemoryLayout->addWidget(m_memorySpinBox,          1, 3, 1, 1, Qt::AlignTop);
    m_machineMemoryLayout->addWidget(m_spinBoxMemoryLabel,     1, 4, 1, 1, Qt::AlignTop);
    m_machineMemoryLayout->addWidget(m_minMemoryLabel,         2, 0, 1, 1, Qt::AlignTop);
    m_machineMemoryLayout->addWidget(m_maxMemorylabel,         2, 2, 1, 1, Qt::AlignTop);

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

/**
 * @brief Machine type configuration tab
 * @param machine, machine to be configured
 * @param enableFields, fields enabled or disabled
 * @param parent, parent widget
 *
 * In this window the user can select the machine type
 */
MachineTypeTab::MachineTypeTab(Machine *machine,
                               bool enableFields,
                               QWidget *parent) : QWidget(parent)
{
    this->m_machine = machine;

    customFilter = new CustomFilter(this);
    this->setMachines();

    filterLabel = new QLabel(tr("Filter:"));

    filterLineEdit = new QLineEdit();
    filterLineEdit->setEnabled(enableFields);
    connect(filterLineEdit, &QLineEdit::textChanged,
            this, &MachineTypeTab::textFilterChanged);

    machinesTypesTreeView = new QTreeView;
    machinesTypesTreeView->setEnabled(enableFields);
    machinesTypesTreeView->setRootIsDecorated(false);
    machinesTypesTreeView->setModel(customFilter);

    // Select the machine type
    bool machineTypeFound = false;
    int machineIndex = 0;
    for(int i = 0; i < machinesTypesTreeView->model()->rowCount() && !machineTypeFound; ++i) {


        QModelIndex index = machinesTypesTreeView->model()->index(i, 0);
        QString machineType = index.data().toString();

        if (this->m_machine->getType() == machineType) {
            machineTypeFound = true;
            machineIndex = i;
        }
    }

    QModelIndex nIndex = machinesTypesTreeView->model()->index(machineIndex, 1);
    machinesTypesTreeView->setCurrentIndex(nIndex);

    machinePageLayout = new QGridLayout;
    machinePageLayout->addWidget(filterLabel, 0, 0);
    machinePageLayout->addWidget(filterLineEdit, 0, 1);
    machinePageLayout->addWidget(machinesTypesTreeView, 1, 0, 1, 3);

    this->setLayout(machinePageLayout);

    qDebug() << "MachineTypeTab created";
}

MachineTypeTab::~MachineTypeTab()
{
    qDebug() << "MachineTypeTab destroyed";
}

void MachineTypeTab::textFilterChanged()
{
    QRegularExpression regExp(this->filterLineEdit->text());
    customFilter->setFilterRegularExpression(regExp);
}

/**
 * @brief Add the machines to the model
 *
 * Add all the machines to the model
 */
void MachineTypeTab::setMachines()
{
    QStandardItemModel *model = new QStandardItemModel(0, 2, this);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Machine"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));

    this->addMachine(model, "pc-q35-2.4", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.5", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.6", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.7", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.8", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.9", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.10", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.11", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.12", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-3.0", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "q35", "Standard PC (Q35 + ICH9, 2009) (alias of pc-q35-3.0)");
    this->addMachine(model, "pc-0.10", "Standard PC (i440FX + PIIX, 1996) (deprecated)");
    this->addMachine(model, "pc-0.11", "Standard PC (i440FX + PIIX, 1996) (deprecated)");
    this->addMachine(model, "pc-0.12", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.13", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.14", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.15", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.0", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.1", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.2", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.3", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.4", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.5", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.6", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.7", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.0", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.1", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.2", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.3", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.4", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.5", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.6", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.7", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.8", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.9", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.10", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.11", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.12", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-3.0", "Standard PC (i440FX + PIIX, 1996) (default)");
    this->addMachine(model, "pc", "Standard PC (i440FX + PIIX, 1996) (alias of pc-i440fx-3.0)");
    this->addMachine(model, "isapc", "ISA-only PC");
    this->addMachine(model, "none", "empty machine");

    customFilter->setSourceModel(model);
}

/**
 * @brief Add the machine to the model
 * @param model, model list with the machines
 * @param machine, code of the machine
 * @param description, description of the machine
 *
 * Add the machine to the model
 */
void MachineTypeTab::addMachine(QAbstractItemModel *model,
                                const QString &machine,
                                const QString &description)
{    
    model->insertRow(0);
    model->setData(model->index(0, 0), machine);
    model->setData(model->index(0, 1), description);
}

/**
 * @brief Get the selected machine type
 * @return selected machine type
 *
 * Get the selected machine type
 */
QString MachineTypeTab::getMachineType()
{
    QString machineType;

    QModelIndexList indexes = this->machinesTypesTreeView->selectionModel()->selectedIndexes();
    if (indexes.size() > 0) {
        QModelIndex selectedIndex = indexes.at(0);

        QString type = selectedIndex.data().toString();

        if (!type.isEmpty() && type != "none") {
            machineType = type;
        }
    }

    return machineType;
}

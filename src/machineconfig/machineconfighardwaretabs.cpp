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

    m_CPUCountLabel = new QLabel(tr("SMP Core count") + ":", this);
    m_CPUCountLabel->setWordWrap(true);

    m_CPUCountSpinBox = new QSpinBox(this);
    m_CPUCountSpinBox->setMinimum(1);
    m_CPUCountSpinBox->setMaximum(255);
    m_CPUCountSpinBox->setValue(machine->getCPUCount());
    m_CPUCountSpinBox->setEnabled(enableFields);

    m_CPUCountLayout = new QHBoxLayout();
    m_CPUCountLayout->setAlignment(Qt::AlignVCenter);
    m_CPUCountLayout->setSpacing(5);
    m_CPUCountLayout->addWidget(m_CPUCountLabel);
    m_CPUCountLayout->addWidget(m_CPUCountSpinBox);

    m_CPUSettingsLayout = new QVBoxLayout();
    m_CPUSettingsLayout->setAlignment(Qt::AlignVCenter);
    m_CPUSettingsLayout->addItem(m_CPUCountLayout);

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
    m_memorySlider->setTickInterval(totalRAM / 8); // Set tick interval to 1/8 of total RAM
    m_memorySlider->setSingleStep(128); // Set slider step to 128 MiB
    m_memorySlider->setMinimum(1);
    m_memorySlider->setMaximum(totalRAM);
    m_memorySlider->setValue(static_cast<int>(machine->getRAM()));
    m_memorySlider->setEnabled(enableFields);

    connect(m_memorySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
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
                               QEMU *qemuObject,
                               QWidget *parent) : QWidget(parent)
{
    this->m_machine = machine;
    this->m_qemuObject = qemuObject;
    
    qDebug() << "MachineTypeTab constructor - QEMU object:" << (qemuObject != nullptr);

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
    qDebug() << "setMachines started";
    
    QStandardItemModel *model = new QStandardItemModel(0, 2, this);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Machine"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));

    // Get machine types from QEMU
    QMap<QString, QString> machineTypes = getMachineTypesFromQEMU(m_qemuObject);
    qDebug() << "Got machine types, count:" << machineTypes.size();
    
    // Add machines to model, filtering deprecated versions
    QMapIterator<QString, QString> i(machineTypes);
    int addedCount = 0;
    while (i.hasNext()) {
        i.next();
        if (!isDeprecatedVersion(i.value())) {
            this->addMachine(model, i.key(), i.value());
            addedCount++;
        }
    }
    qDebug() << "Added machines to model:" << addedCount;

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
QMap<QString, QString> MachineTypeTab::getMachineTypesFromQEMU(QEMU *qemuObject)
{
    QMap<QString, QString> machineTypes;
    
    if (!qemuObject) {
        qDebug() << "Error: QEMU object is null";
        return machineTypes;
    }
    
    QString qemuBinary = qemuObject->getQEMUBinary("qemu-system-x86_64");
    qDebug() << "QEMU binary path:" << qemuBinary;
    
    if (qemuBinary.isEmpty()) {
        qDebug() << "Error: Could not find QEMU binary";
        return machineTypes;
    }

    QProcess process;
    process.start(qemuBinary, QStringList() << "-machine" << "?");
    bool started = process.waitForStarted();
    qDebug() << "Process started:" << started;
    
    bool finished = process.waitForFinished();
    qDebug() << "Process finished:" << finished;
    
    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();
    
    qDebug() << "QEMU output length:" << output.length();
    if (!error.isEmpty()) {
        qDebug() << "QEMU error:" << error;
    }
    
    QStringList lines = output.split("\n");
    qDebug() << "Number of lines:" << lines.size();
    
    // Skip first line which contains "Supported machines are:"
    for (int i = 1; i < lines.size(); i++) {
        QString line = lines[i].trimmed();
        if (line.isEmpty()) continue;
        
        // Parse each line in format: "name    description"
        int firstSpace = line.indexOf(" ");
        if (firstSpace > 0) {
            QString name = line.left(firstSpace);
            QString description = line.mid(firstSpace).trimmed();
            machineTypes[name] = description;
        }
    }
    
    qDebug() << "Found machine types:" << machineTypes.size();
    return machineTypes;
}

bool MachineTypeTab::isDeprecatedVersion(const QString &description)
{
    return description.contains("(deprecated)", Qt::CaseInsensitive);
}

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

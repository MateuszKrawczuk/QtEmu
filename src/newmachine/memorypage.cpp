// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "memorypage.h"

/**
 * @brief Memory page
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Memory page section. In this page you can select the amount of memory for
 * the new machine
 */
MachineMemoryPage::MachineMemoryPage(Machine *machine,
                                     QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine memory"));
    this->m_newMachine = machine;

    m_descriptionMemoryLabel = new QLabel(
                tr("Select the amount of base memory (RAM) in megabytes for virtual machine allocating."), this);
    m_descriptionMemoryLabel->setWordWrap(true);

    int totalRAM = 0;
    SystemUtils::getTotalMemory(totalRAM);
    int defaultRAM = totalRAM / 4;
    m_spinBoxMemoryLabel = new QLabel("MiB", this);

    m_memorySpinBox = new QSpinBox(this);
    m_memorySpinBox->setMinimum(1);
    m_memorySpinBox->setMaximum(totalRAM);

    m_memorySpinBox->setValue(defaultRAM);
    this->registerField("machine.ram", m_memorySpinBox, "value", "valueChanged");

    m_memorySlider = new QSlider(Qt::Horizontal, this);
    m_memorySlider->setTickPosition(QSlider::TicksBelow);
    m_memorySlider->setTickInterval(totalRAM / 8); // Set tick interval to 1/8 of total RAM
    m_memorySlider->setSingleStep(128); // Set slider step to 128 MiB
    m_memorySlider->setMinimum(1);
    m_memorySlider->setMaximum(totalRAM);

    m_memorySlider->setValue(defaultRAM);

    connect(m_memorySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_memorySlider, &QSlider::setValue);

    connect(m_memorySlider, &QSlider::valueChanged,
            m_memorySpinBox, &QSpinBox::setValue);

    m_minMemoryLabel = new QLabel("1 MiB", this);
    m_maxMemorylabel = new QLabel(QString("%1 MiB").arg(totalRAM), this);

    // Create memory settings group
    m_memorySettings = new QGroupBox(tr("RAM Settings"), this);
    
    // Create layout for the group box contents
    QGridLayout *memorySettingsLayout = new QGridLayout();
    memorySettingsLayout->setColumnStretch(1, 50);
    memorySettingsLayout->addWidget(m_memorySlider,       0, 0, 1, 3);
    memorySettingsLayout->addWidget(m_memorySpinBox,      0, 3, 1, 1);
    memorySettingsLayout->addWidget(m_spinBoxMemoryLabel, 0, 4, 1, 1);
    memorySettingsLayout->addWidget(m_minMemoryLabel,     1, 0, 1, 1);
    memorySettingsLayout->addWidget(m_maxMemorylabel,     1, 2, 1, 1);
    
    // Set the layout for the group box
    m_memorySettings->setLayout(memorySettingsLayout);
    
    // Update main layout
    m_machineMemoryLayout = new QGridLayout();
    m_machineMemoryLayout->setColumnStretch(1, 50);
    m_machineMemoryLayout->addWidget(m_descriptionMemoryLabel, 0, 0, 1, 5);
    m_machineMemoryLayout->addWidget(m_memorySettings,         1, 0, 1, 5);

    this->setLayout(m_machineMemoryLayout);

    qDebug() << "MachineMemoryPage created";
}

MachineMemoryPage::~MachineMemoryPage()
{
    qDebug() << "MachineMemoryPage destroyed";
}

bool MachineMemoryPage::validatePage()
{
    this->m_newMachine->setRAM(this->m_memorySpinBox->value());
    return true;
}

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
#include "conclusionpage.h"

/**
 * @brief Conclusion page
 * @param machine, new machine object
 * @param osListWidget, list with all the machines from the mainwindow
 * @param QEMUGlobalObject, QEMU global object with data about QEMU
 * @param parent, widget parent
 *
 * Conclusion page section. In this page a summary of the new machine
 * is shown.
 */
MachineConclusionPage::MachineConclusionPage(Machine *machine,
                                             QListWidget *osListWidget,
                                             QEMU *QEMUGlobalObject,
                                             QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine Summary"));
    this->m_newMachine = machine;
    this->m_QEMUGlobalObject = QEMUGlobalObject;
    this->m_osList = osListWidget;

    m_conclusionLabel = new QLabel(tr("Summary of the new machine"), this);
    m_machineDescLabel = new QLabel(tr("Name") + ":", this);
    m_OSTypeDescLabel = new QLabel(tr("Operating system") + ":", this);
    m_OSVersionDescLabel = new QLabel(tr("Operating system version") + ":", this);
    m_processorDescLabel = new QLabel(tr("Processor") + ":", this);
    m_graphicsDescLabel = new QLabel(tr("Graphics") + ":", this);
    m_audioDescLabel = new QLabel(tr("Audio") + ":", this);
    m_RAMDescLabel = new QLabel(tr("RAM") + ":", this);
    m_acceleratorDescLabel = new QLabel(tr("Accelerator") + ":", this);
    m_diskDescLabel = new QLabel(tr("Disk") + ":");

    m_machineNameLabel = new QLabel(this);
    m_machineNameLabel->setWordWrap(true);
    m_OSTypeLabel      = new QLabel(this);
    m_OSVersionLabel   = new QLabel(this);
    m_processorLabel   = new QLabel(this);
    m_graphicsLabel    = new QLabel(this);
    m_audioLabel       = new QLabel(this);
    m_audioLabel->setWordWrap(true);
    m_RAMLabel         = new QLabel(this);
    m_acceleratorLabel = new QLabel(this);
    m_acceleratorLabel->setWordWrap(true);
    m_diskLabel        = new QLabel(this);
    m_diskLabel->setWordWrap(true);

    m_conclusionLayout = new QGridLayout();
    m_conclusionLayout->addWidget(m_conclusionLabel,      0, 0, 1, 4);
    m_conclusionLayout->addWidget(m_machineDescLabel,     1, 0, 1, 1);
    m_conclusionLayout->addWidget(m_machineNameLabel,     1, 1, 1, 1);
    m_conclusionLayout->addWidget(m_OSTypeDescLabel,      2, 0, 1, 1);
    m_conclusionLayout->addWidget(m_OSTypeLabel,          2, 1, 1, 1);
    m_conclusionLayout->addWidget(m_OSVersionDescLabel,   3, 0, 1, 1);
    m_conclusionLayout->addWidget(m_OSVersionLabel,       3, 1, 1, 1);
    m_conclusionLayout->addWidget(m_processorDescLabel,   4, 0, 1, 1);
    m_conclusionLayout->addWidget(m_processorLabel,       4, 1, 1, 1);
    m_conclusionLayout->addWidget(m_graphicsDescLabel,    5, 0, 1, 1);
    m_conclusionLayout->addWidget(m_graphicsLabel,        5, 1, 1, 1);
    m_conclusionLayout->addWidget(m_audioDescLabel,       6, 0, 1, 1);
    m_conclusionLayout->addWidget(m_audioLabel,           6, 1, 1, 3);
    m_conclusionLayout->addWidget(m_RAMDescLabel,         7, 0, 1, 1);
    m_conclusionLayout->addWidget(m_RAMLabel,             7, 1, 1, 1);
    m_conclusionLayout->addWidget(m_acceleratorDescLabel, 8, 0, 1, 1);
    m_conclusionLayout->addWidget(m_acceleratorLabel,     8, 1, 1, 1);

    this->setLayout(m_conclusionLayout);

    qDebug() << "MachineConclusionPage created";
}

MachineConclusionPage::~MachineConclusionPage()
{
    qDebug() << "MachineConclusionPage destroyed";
}

void MachineConclusionPage::initializePage()
{
    QString diskName = field("machine.diskname").toString();

    this->m_machineNameLabel->setText(this->m_newMachine->getName());
    this->m_OSTypeLabel->setText(this->m_newMachine->getOSType());
    this->m_OSVersionLabel->setText(this->m_newMachine->getOSVersion());
    this->m_processorLabel->setText(this->m_newMachine->getCPUType());
    this->m_graphicsLabel->setText(this->m_newMachine->getGPUType());
    this->m_RAMLabel->setText(QString::number(this->m_newMachine->getRAM()).append(" MiB"));
    this->m_audioLabel->setText(this->m_newMachine->getAudioLabel());
    this->m_acceleratorLabel->setText(this->m_newMachine->getAcceleratorLabel());

    if (!diskName.isEmpty()) {
        this->m_diskLabel->setText(diskName);
        this->m_conclusionLayout->addWidget(this->m_diskDescLabel,    9, 0, 1, 1);
        this->m_conclusionLayout->addWidget(this->m_diskLabel,        9, 1, 1, 1);
    } else {
        this->m_diskLabel->setText("");
        this->m_conclusionLayout->removeWidget(this->m_diskDescLabel);
        this->m_conclusionLayout->removeWidget(this->m_diskLabel);
    }

}

bool MachineConclusionPage::validatePage()
{
    // Data to crontol the disk
    bool createNewDisk = field("createDisk").toBool();
    bool useDisk = field("useDisk").toBool();
    QString existingDiskPath = field("machine.diskPath").toString();
    QString diskName = field("machine.diskname").toString();
    QString diskFormat = field("machine.diskFormat").toString();
    double diskSize = field("machine.diskSize").toDouble();

    QSettings settings;
    settings.beginGroup("Configuration");
    QString machinesPath = settings.value("machinePath", QDir::homePath()).toString();
    machinesPath.append(QDir::toNativeSeparators("/"))
                        .append(this->m_newMachine->getName())
                        .append(QDir::toNativeSeparators("/"));
    settings.endGroup();

    QString machineConfigPath = machinesPath;
    this->m_newMachine->setConfigPath(machineConfigPath
                                      .append(this->m_newMachine->getName().toLower().replace(" ", "_"))
                                      .append(".json"));

    if (createNewDisk) {
        QString diskPathName = machinesPath;
        diskPathName.append(diskName.toLower().replace(" ", "_"))
                    .append(QDir::toNativeSeparators("."))
                    .append(diskFormat);

        bool isDiskCreated = SystemUtils::createDisk(this->m_QEMUGlobalObject,
                                                     diskPathName,
                                                     diskFormat,
                                                     diskSize,
                                                     false);
        if (isDiskCreated) {
            this->addMedia(diskName.toLower().replace(" ", "_"), diskPathName);
        } else {
            // If the creation of the disk fails
            return false;
        }
    } else if (useDisk) {
        if (!existingDiskPath.isEmpty()) {
            // Add the existing media to the machine media
            QFileInfo existingDisk(existingDiskPath);
            this->addMedia(existingDisk.fileName(), existingDisk.filePath());
        }
    }

    // Set the uuid in this point to control the LoadUI in the mainwindow
    // don't move from here
    this->m_newMachine->setUuid(QUuid::createUuid().toString());
    this->generateMachineFiles();
    return true;
}

/**
 * @brief Generate the machine files
 *
 * Generate the necessary machine files
 */
void MachineConclusionPage::generateMachineFiles()
{
    this->generateBoot();
    this->m_newMachine->saveMachine();
    this->m_newMachine->insertMachineConfigFile();
    this->insertVMList();

    Logger::logMachineCreation(this->m_newMachine->getPath(),
                               this->m_newMachine->getName(), "Machine created");
}

/**
 * @brief Insert the new VM in te list
 *
 * Insert the new VM in te list that shows in the mainwindow
 */
void MachineConclusionPage::insertVMList()
{
    QListWidgetItem *machine = new QListWidgetItem(this->m_newMachine->getName(), this->m_osList);

    machine->setData(QMetaType::QUuid, this->m_newMachine->getUuid());
    // TODO: Check if the json it's incomplete and the image not exits
    machine->setIcon(QIcon(":/images/os/64x64/" +
                             SystemUtils::getOsIcon(this->m_newMachine->getOSVersion())));
}

/**
 * @brief Add media
 * @param name, name for the new media
 * @param path, path where the media is located
 *
 * Add media to the machine
 */
void MachineConclusionPage::addMedia(const QString name, const QString path)
{
    Media disk;
    disk.setName(name);
    disk.setPath(path);
    disk.setType("hdd");
    disk.setDriveInterface("hda");
    disk.setUuid(QUuid::createUuid().toString());

    this->m_newMachine->addMedia(disk);
}

/**
 * @brief Generate a default boot
 *
 * Generate a default boot for the machine.
 * Boot from HDD first
 */
void MachineConclusionPage::generateBoot()
{
    Boot boot;
    boot.setBootMenu(false);
    boot.setKernelBootEnabled(false);
    boot.setKernelPath("");
    boot.setInitrdPath("");
    boot.setKernelArgs("");
    boot.addBootOrder("c"); // Boot from HDD

    this->m_newMachine->setBoot(boot);
}

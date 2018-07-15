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

MachineConclusionPage::MachineConclusionPage(Machine *machine, QListWidget *osListWidget,
                                             QWidget *parent) : QWizardPage(parent) {

    this -> setTitle(tr("Machine Summary"));

    this -> m_newMachine = machine;
    this -> m_osList = osListWidget;

    m_conclusionLabel = new QLabel(tr("Summary of the new machine"));
    m_machineDescLabel = new QLabel(tr("Name") + ":");
    m_OSTypeDescLabel = new QLabel(tr("Operating system") + ":");
    m_OSVersionDescLabel = new QLabel(tr("Operating system version") + ":");
    m_processorDescLabel = new QLabel(tr("Processor") + ":");
    m_graphicsDescLabel = new QLabel(tr("Graphics") + ":");
    m_audioDescLabel = new QLabel(tr("Audio") + ":");
    m_RAMDescLabel = new QLabel(tr("RAM") + ":");
    m_acceleratorDescLabel = new QLabel(tr("Accelerator") + ":");
    m_diskDescLabel = new QLabel(tr("Disk") + ":");

    m_machineNameLabel = new QLabel();
    m_machineNameLabel -> setWordWrap(true);
    m_OSTypeLabel      = new QLabel();
    m_OSVersionLabel   = new QLabel();
    m_processorLabel   = new QLabel();
    m_graphicsLabel    = new QLabel();
    m_audioLabel       = new QLabel();
    m_audioLabel -> setWordWrap(true);
    m_RAMLabel         = new QLabel();
    m_acceleratorLabel = new QLabel();
    m_acceleratorLabel -> setWordWrap(true);
    m_diskLabel        = new QLabel();
    m_diskLabel -> setWordWrap(true);

    m_conclusionLayout = new QGridLayout();

    m_conclusionLayout -> addWidget(m_conclusionLabel,      0, 0, 1, 4);
    m_conclusionLayout -> addWidget(m_machineDescLabel,     1, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_machineNameLabel,     1, 1, 1, 1);
    m_conclusionLayout -> addWidget(m_OSTypeDescLabel,      2, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_OSTypeLabel,          2, 1, 1, 1);
    m_conclusionLayout -> addWidget(m_OSVersionDescLabel,   3, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_OSVersionLabel,       3, 1, 1, 1);
    m_conclusionLayout -> addWidget(m_processorDescLabel,   4, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_processorLabel,       4, 1, 1, 1);
    m_conclusionLayout -> addWidget(m_graphicsDescLabel,    5, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_graphicsLabel,        5, 1, 1, 1);
    m_conclusionLayout -> addWidget(m_audioDescLabel,       6, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_audioLabel,           6, 1, 1, 3);
    m_conclusionLayout -> addWidget(m_RAMDescLabel,         7, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_RAMLabel,             7, 1, 1, 1);
    m_conclusionLayout -> addWidget(m_acceleratorDescLabel, 8, 0, 1, 1);
    m_conclusionLayout -> addWidget(m_acceleratorLabel,     8, 1, 1, 1);

    this -> setLayout(m_conclusionLayout);

    qDebug() << "MachineConclusionPage created";
}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

void MachineConclusionPage::initializePage() {
    this -> m_machineNameLabel -> setText(this -> m_newMachine -> getName());
    this -> m_OSTypeLabel      -> setText(this -> m_newMachine -> getOSType());
    this -> m_OSVersionLabel   -> setText(this -> m_newMachine -> getOSVersion());
    this -> m_processorLabel   -> setText(this -> m_newMachine -> getCPUType());
    this -> m_graphicsLabel    -> setText(this -> m_newMachine -> getGPUType());
    this -> m_RAMLabel         -> setText(QString::number(this -> m_newMachine -> getRAM()).append(" MiB"));
    this -> m_audioLabel       -> setText(this -> m_newMachine -> getAudioLabel());
    this -> m_acceleratorLabel -> setText(this -> m_newMachine -> getAcceleratorLabel());
    this -> m_diskLabel        -> setText(this -> m_newMachine -> getDiskName());

    if( ! this -> m_newMachine -> getDiskName().isEmpty()){
        this -> m_conclusionLayout -> addWidget(this -> m_diskDescLabel,    9, 0, 1, 1);
        this -> m_conclusionLayout -> addWidget(this -> m_diskLabel,        9, 1, 1, 1);
    } else {
        this -> m_conclusionLayout -> removeWidget(this -> m_diskDescLabel);
        this -> m_conclusionLayout -> removeWidget(this -> m_diskLabel);
    }

}

bool MachineConclusionPage::validatePage() {

    this -> m_newMachine -> setUuid(QUuid::createUuid().toString());

    if( ! this -> m_newMachine -> getCreateNewDisk() ) {
        this -> insertVMList();
        return true;
    }

    QString strMachinePath;

    if ( ! this -> m_newMachine -> getDiskPath().isEmpty()) {

        strMachinePath = this -> m_newMachine -> getDiskPath();

    } else {
        QSettings settings;
        settings.beginGroup("Configuration");

        strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

        settings.endGroup();

        strMachinePath.append(QDir::toNativeSeparators("/"))
                      .append(this -> m_newMachine -> getName())
                      .append(QDir::toNativeSeparators("/"))
                      .append(this -> m_newMachine -> getDiskName().toLower().replace(" ", "_"))
                      .append(QDir::toNativeSeparators("."))
                      .append(this -> m_newMachine -> getDiskFormat());
    }

    this -> m_newMachine -> setDiskPath(strMachinePath);

    // Create the disk
    bool isDiskCreated = SystemUtils::createDisk(strMachinePath,
                                                 this -> m_newMachine -> getDiskFormat(),
                                                 this -> m_newMachine -> getDiskSize(),
                                                 false);

    if (isDiskCreated) {

        this -> createMachineJSON(this -> m_newMachine);

        this -> insertOSInFile(this -> m_newMachine);

        this -> insertVMList();

        Logger::logMachineCreation(this -> m_newMachine -> getPath(),
                                   this -> m_newMachine -> getName(), "Machine created");
    }

    return isDiskCreated;
}

/**
 * @brief Insert the new VM in te list
 *
 * Insert the new VM in te list
 */
void MachineConclusionPage::insertVMList() {

    QListWidgetItem *machine = new QListWidgetItem(this -> m_newMachine -> getName(), this -> m_osList);

    machine -> setData(QMetaType::QUuid, this -> m_newMachine -> getUuid());
    // TODO: Check if the json it's incomplete and the image not exits
    machine -> setIcon(QIcon(":/images/os/64x64/" +
                             SystemUtils::getOsIcon(this -> m_newMachine -> getOSVersion())));

}

void MachineConclusionPage::createMachineJSON(Machine *machine) const {

    QSettings settings;
    settings.beginGroup("Configuration");

    QString strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

    settings.endGroup();

    QString machineFilePath = strMachinePath.append(QDir::toNativeSeparators("/"))
                                            .append(machine -> getName())
                                            .append(QDir::toNativeSeparators("/"))
                                            .append(machine -> getName().toLower().replace(" ", "_"))
                                            .append(".json");

    QFile machineFile(machineFilePath);
    machineFile.open(QIODevice::WriteOnly); // TODO: Check if open the file fails

    QJsonObject machineJSONObject;
    this -> fillMachineJSON(machineJSONObject); // Populate the JSON Object

    QJsonDocument machineJSONDocument(machineJSONObject);

    machineFile.write(machineJSONDocument.toJson());
    machine -> setConfigPath(machineFilePath);
}

void MachineConclusionPage::fillMachineJSON(QJsonObject &machineJSONObject) const{

    machineJSONObject["name"]      = this -> m_newMachine -> getName();
    machineJSONObject["OSType"]    = this -> m_newMachine -> getOSType();
    machineJSONObject["OSVersion"] = this -> m_newMachine -> getOSVersion();
    machineJSONObject["RAM"]       = this -> m_newMachine -> getRAM();
    machineJSONObject["network"]   = this -> m_newMachine -> getUseNetwork();
    machineJSONObject["path"]      = this -> m_newMachine -> getPath();
    machineJSONObject["uuid"]      = this -> m_newMachine -> getUuid();

    QJsonObject cpu;
    cpu["CPUType"]     = this -> m_newMachine -> getCPUType();
    cpu["CPUCount"]    = this -> m_newMachine -> getCPUCount();
    cpu["socketCount"] = this -> m_newMachine -> getSocketCount();
    cpu["coresSocket"] = this -> m_newMachine -> getCoresSocket();
    cpu["threadsCore"] = this -> m_newMachine -> getThreadsCore();
    cpu["maxHotCPU"]   = this -> m_newMachine -> getMaxHotCPU();

    machineJSONObject["cpu"] = cpu;

    QJsonObject gpu;
    gpu["GPUType"]  = this -> m_newMachine -> getGPUType();
    gpu["keyboard"] = this -> m_newMachine -> getKeyboard();

    machineJSONObject["gpu"] = gpu;

    QJsonObject disk;
    disk["name"] = this -> m_newMachine -> getDiskName();
    disk["path"] = this -> m_newMachine -> getDiskPath();
    disk["size"] = this -> m_newMachine -> getDiskSize();
    disk["type"] = "hdd";
    disk["format"] = this -> m_newMachine -> getDiskFormat();
    disk["interface"] = "hda";
    disk["uuid"] = QUuid::createUuid().toString();

    QJsonArray media;
    media.append(disk);

    machineJSONObject["media"] = media;

    QJsonObject kernelBoot;
    kernelBoot["enabled"] = false;
    kernelBoot["kernelPath"] = "";
    kernelBoot["initrdPath"] = "";
    kernelBoot["kernelArgs"] = "";

    QJsonObject bootOrder;
    bootOrder["0"] = "CDROM";
    bootOrder["1"] = "HDD";

    QJsonObject boot;
    boot["bootMenu"] = false;
    boot["kernelBoot"] = kernelBoot;
    boot["bootOrder"] = bootOrder;

    machineJSONObject["boot"] = boot;

    if( ! this -> m_newMachine -> getAccelerator().isEmpty()) { 
        machineJSONObject["accelerator"] = QJsonArray::fromStringList(this -> m_newMachine -> getAccelerator());
    }

    if( ! this -> m_newMachine -> getAudio().isEmpty()) {
        machineJSONObject["audio"] = QJsonArray::fromStringList(this -> m_newMachine -> getAudio());
    }

}

/**
 * @brief Insert the new machine in the machines file
 * @param newMachine, machine to be inserted
 *
 * Insert the new machine in the machines file.
 * At the bottom of the file.
 */
void MachineConclusionPage::insertOSInFile(Machine *newMachine){

    // TODO: Get the data directory path from QSettings
    // Open the file
    QString dataDirectoryPath = QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/");

    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all data included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonObject machinesObject;

    // Read other machines
    QJsonArray machines = machinesDocument["machines"].toArray();

    // Create the new machine
    QJsonObject machine;
    machine["uuid"]       = newMachine -> getUuid();
    machine["name"]       = newMachine -> getName();
    machine["path"]       = newMachine -> getPath();
    machine["configpath"] = newMachine -> getConfigPath();
    machine["icon"]       = newMachine -> getOSVersion().toLower().replace(" ", "_");

    machines.append(machine);
    machinesObject["machines"] = machines;

    QJsonDocument machinesJSONDocument(machinesObject);

    machinesFile.seek(0);
    machinesFile.write(machinesJSONDocument.toJson());
    machinesFile.close();
}

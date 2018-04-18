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

// Qt

// Local
#include "conclusionpage.h"

MachineConclusionPage::MachineConclusionPage(Machine *machine, QListWidget *osListWidget,
                                             QWidget *parent) : QWizardPage(parent) {

    this -> setTitle(tr("Machine Summary"));

    this -> newMachine = machine;
    this -> osList = osListWidget;

    conclusionLabel = new QLabel(tr("Summary of the new machine"));

    machineDescLabel = new QLabel(tr("Name") + ":");
    OSTypeDescLabel = new QLabel(tr("Operating system") + ":");
    OSVersionDescLabel = new QLabel(tr("Operating system version") + ":");
    processorDescLabel = new QLabel(tr("Processor") + ":");
    graphicsDescLabel = new QLabel(tr("Graphics") + ":");
    audioDescLabel = new QLabel(tr("Audio") + ":");
    RAMDescLabel = new QLabel(tr("RAM") + ":");
    acceleratorDescLabel = new QLabel(tr("Accelerator") + ":");
    diskDescLabel = new QLabel(tr("Disk") + ":");

    machineNameLabel = new QLabel();
    machineNameLabel -> setWordWrap(true);
    OSTypeLabel      = new QLabel();
    OSVersionLabel   = new QLabel();
    processorLabel   = new QLabel();
    graphicsLabel    = new QLabel();
    audioLabel       = new QLabel();
    audioLabel -> setWordWrap(true);
    RAMLabel         = new QLabel();
    acceleratorLabel = new QLabel();
    diskLabel        = new QLabel();
    diskLabel -> setWordWrap(true);

    conclusionLayout = new QGridLayout();

    conclusionLayout -> setColumnStretch(1, 10);
    conclusionLayout -> setColumnStretch(2, 10);
    conclusionLayout -> setColumnStretch(3, 10);

    conclusionLayout -> addWidget(conclusionLabel,      0, 0, 1, 4);
    conclusionLayout -> addWidget(machineDescLabel,     1, 0, 1, 1);
    conclusionLayout -> addWidget(machineNameLabel,     1, 1, 1, 1);
    conclusionLayout -> addWidget(OSTypeDescLabel,      2, 0, 1, 1);
    conclusionLayout -> addWidget(OSTypeLabel,          2, 1, 1, 1);
    conclusionLayout -> addWidget(OSVersionDescLabel,   3, 0, 1, 1);
    conclusionLayout -> addWidget(OSVersionLabel,       3, 1, 1, 1);
    conclusionLayout -> addWidget(processorDescLabel,   4, 0, 1, 1);
    conclusionLayout -> addWidget(processorLabel,       4, 1, 1, 1);
    conclusionLayout -> addWidget(graphicsDescLabel,    5, 0, 1, 1);
    conclusionLayout -> addWidget(graphicsLabel,        5, 1, 1, 1);
    conclusionLayout -> addWidget(audioDescLabel,       6, 0, 1, 1);
    conclusionLayout -> addWidget(audioLabel,           6, 1, 1, 3);
    conclusionLayout -> addWidget(RAMDescLabel,         7, 0, 1, 1);
    conclusionLayout -> addWidget(RAMLabel,             7, 1, 1, 1);
    conclusionLayout -> addWidget(acceleratorDescLabel, 8, 0, 1, 1);
    conclusionLayout -> addWidget(acceleratorLabel,     8, 1, 1, 1);

    this -> setLayout(conclusionLayout);

    qDebug() << "MachineConclusionPage created";
}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

void MachineConclusionPage::initializePage() {
    this -> machineNameLabel -> setText(this -> newMachine -> getName());
    this -> OSTypeLabel      -> setText(this -> newMachine -> getOSType());
    this -> OSVersionLabel   -> setText(this -> newMachine -> getOSVersion());
    this -> processorLabel   -> setText(this -> newMachine -> getCPUType());
    this -> graphicsLabel    -> setText(this -> newMachine -> getGPUType());
    this -> RAMLabel         -> setText(QString::number(this -> newMachine -> getRAM()).append(" MiB"));
    this -> audioLabel       -> setText(this -> newMachine -> getAudioLabel());
    this -> acceleratorLabel -> setText(this -> newMachine -> getAcceleratorLabel());
    this -> diskLabel        -> setText(this -> newMachine -> getDiskName());

    if( ! this -> newMachine -> getDiskName().isEmpty()){
        this -> conclusionLayout -> addWidget(this -> diskDescLabel,    9, 0, 1, 1);
        this -> conclusionLayout -> addWidget(this -> diskLabel,        9, 1, 1, 1);
    } else {
        this -> conclusionLayout -> removeWidget(this -> diskDescLabel);
        this -> conclusionLayout -> removeWidget(this -> diskLabel);
    }

}

bool MachineConclusionPage::validatePage() {

    this -> newMachine -> setUuid(QUuid::createUuid().toString());

    // Add the new machine to the list
    QListWidgetItem *machine = new QListWidgetItem(this -> newMachine -> getName(), this -> osList);
    machine -> setData(QMetaType::QUuid, this -> newMachine -> getUuid());
    // TODO: Check if the json it's incomplete and the image not exits
    machine -> setIcon(QIcon(":/images/os/64x64/" +
                             SystemUtils::getOsIcon(this -> newMachine -> getOSVersion())));

    if( ! this -> newMachine -> getCreateNewDisk() ) {
        return true;
    }

    QString strMachinePath;

    if ( ! this -> newMachine -> getDiskPath().isEmpty()) {

        strMachinePath = this -> newMachine -> getDiskPath();

    } else {
        QSettings settings;
        settings.beginGroup("Configuration");

        strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

        settings.endGroup();

        strMachinePath.append("/")
                      .append(this -> newMachine -> getName())
                      .append("/")
                      .append(this -> newMachine -> getDiskName().toLower().replace(" ", "_"))
                      .append(".")
                      .append(this -> newMachine -> getDiskFormat());
    }

    this -> newMachine -> setDiskPath(strMachinePath);

    // Create the disk
    bool isDiskCreated = SystemUtils::createDisk(strMachinePath,
                                                 this -> newMachine -> getDiskFormat(),
                                                 this -> newMachine -> getDiskSize(),
                                                 false);

    if (isDiskCreated) {

        createMachineJSON(this -> newMachine);

        SystemUtils::populateOSList(this -> newMachine);

        Logger::logMachineCreation(this -> newMachine -> getPath(),
                                   this -> newMachine -> getName(), "Machine created");
    }

    return isDiskCreated;
}

void MachineConclusionPage::createMachineJSON(Machine *machine) const {

    QSettings settings;
    settings.beginGroup("Configuration");

    QString strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

    settings.endGroup();

    QString machineFilePath = strMachinePath.append("/")
                                            .append(machine -> getName())
                                            .append("/")
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

    machineJSONObject["name"]      = this -> newMachine -> getName();
    machineJSONObject["OSType"]    = this -> newMachine -> getOSType();
    machineJSONObject["OSVersion"] = this -> newMachine -> getOSVersion();
    machineJSONObject["RAM"]       = this -> newMachine -> getRAM();
    machineJSONObject["network"]   = this -> newMachine -> getUseNetwork();
    machineJSONObject["path"]      = this -> newMachine -> getPath();
    machineJSONObject["uuid"]      = this -> newMachine -> getUuid();

    QJsonObject cpu;
    cpu["CPUType"]     = this -> newMachine -> getCPUType();
    cpu["CPUCount"]    = this -> newMachine -> getCPUCount();
    cpu["socketCount"] = this -> newMachine -> getSocketCount();
    cpu["coresSocket"] = this -> newMachine -> getCoresSocket();
    cpu["threadsCore"] = this -> newMachine -> getThreadsCore();
    cpu["maxHotCPU"]   = this -> newMachine -> getMaxHotCPU();

    machineJSONObject["cpu"] = cpu;

    QJsonObject gpu;
    gpu["GPUType"]  = this -> newMachine -> getGPUType();
    gpu["keyboard"] = this -> newMachine -> getKeyboard();

    machineJSONObject["gpu"] = gpu;

    QJsonObject disk;
    disk["name"] = this -> newMachine -> getDiskName();
    disk["path"] = this -> newMachine -> getDiskPath();

    machineJSONObject["disk"] = disk;

    QJsonArray accelerator;
    QStringList acceleratorList = this -> newMachine -> getAcceleratorLabel().split(",");

    for (const auto& i : acceleratorList) {
        if( ! i.isEmpty() ){
            accelerator.append(i.trimmed());
        }
    }

    if( ! accelerator.isEmpty()) {
        machineJSONObject["accelerator"] = accelerator;
    }

    QJsonArray audio;
    QStringList audioList = this -> newMachine -> getAudioLabel().split(",");

    for (const auto& i : audioList) {
        if( ! i.isEmpty() ){
            audio.append(i.trimmed());
        }
    }

    if( ! audio.isEmpty()) {
        machineJSONObject["audio"] = audio;
    }

}


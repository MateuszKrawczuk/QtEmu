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
#include "machineutils.h"

MachineUtils::MachineUtils(QObject *parent) : QObject(parent) {
    qDebug() << "MachineUtils object created";
}

MachineUtils::~MachineUtils() {
    qDebug() << "MachineUtils object destroyed";
}

QStringList MachineUtils::generateMachineCommand(const QUuid machineUuid) {

    QString machineConfigPath = getMachineConfigPath(machineUuid);

    QFile machineFile(machineConfigPath);
    machineFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all data included in the file
    QByteArray machineData = machineFile.readAll();
    QJsonDocument machineDocument(QJsonDocument::fromJson(machineData));
    QJsonObject machineObject = machineDocument.object();
    QJsonObject cpuObject = machineObject["cpu"].toObject();
    QJsonObject gpuObject = machineObject["gpu"].toObject();
    QJsonObject diskObject = machineObject["disk"].toObject();
    QJsonArray accelerator = machineObject["accelerator"].toArray();
    QJsonArray audio = machineObject["audio"].toArray();

    // TODO: Add qemu before commands

    QStringList qemuCommand;

    qemuCommand << "-monitor" << "stdio";

    qemuCommand << "-name";
    qemuCommand << machineObject["name"].toString();

    qemuCommand << "-uuid";
    qemuCommand << machineObject["uuid"].toString().remove("{").remove("}");

    QString accelerators;
    bool firstAccel = true;
    for(int index = 0; index < accelerator.size(); ++index) {
        if(firstAccel) {
            firstAccel = false;
        } else {
            accelerators.append(", ");
        }
        accelerators.append(accelerator[index].toString());
    }

    qemuCommand << "-accel";
    qemuCommand << accelerators;

    QString audioCards;
    bool firstAudio = true;
    for(int index = 0; index < audio.size(); ++index) {
        if(firstAudio) {
            firstAudio = false;
        } else {
            accelerators.append(",");
        }
        audioCards.append(audio[index].toString());
    }

    qemuCommand << "-soundhw";
    qemuCommand << audioCards;

    qemuCommand << "-m";
    qemuCommand << QString::number(machineObject["RAM"].toDouble());

    qemuCommand << "-k";
    qemuCommand << gpuObject["keyboard"].toString();

    qemuCommand << "-vga";
    qemuCommand << gpuObject["GPUType"].toString();

    // CPU
    qemuCommand << "-cpu";
    qemuCommand << cpuObject["CPUType"].toString();

    QString cpuArgs(QString::number(cpuObject["CPUCount"].toInt()));

    if (cpuObject["coresSocket"].toInt() > 0) {
        cpuArgs.append(",cores=");
        cpuArgs.append(QString::number(cpuObject["coresSocket"].toInt()));
    }

    if (cpuObject["threadsCore"].toInt() > 0) {
        cpuArgs.append(",threads=");
        cpuArgs.append(QString::number(cpuObject["threadsCore"].toInt()));
    }

    if (cpuObject["socketCount"].toInt() > 0) {
        cpuArgs.append(",sockets=");
        cpuArgs.append(QString::number(cpuObject["socketCount"].toInt()));
    }

    if (cpuObject["maxHotCPU"].toInt() > 0) {
        cpuArgs.append(",maxcpus=");
        cpuArgs.append(QString::number(cpuObject["maxHotCPU"].toInt()));
    }

    qemuCommand << "-smp";
    qemuCommand << cpuArgs;

    QString pipe = machineObject["path"].toString()
                   .append(QDir::toNativeSeparators("/"))
                   .append(machineObject["name"].toString());

    qemuCommand << "-pidfile";
    qemuCommand << pipe.append(".pid");

    // Network
    bool networkEnabled = machineObject["network"].toBool();
    if (networkEnabled) {
        qemuCommand << "-net";
        qemuCommand << "nic";

        qemuCommand << "-net";
        qemuCommand << "user";
    } else {
        qemuCommand << "-net";
        qemuCommand << "none";
    }

    // HDD
    //qemuCommand << "-drive";
    //qemuCommand << QString("file=").append(diskObject["path"].toString()).append(",index=0,media=disk");

    // CDROM TODO, for test not implemented yet :'(
    qemuCommand << "-cdrom";
    qemuCommand << "/home/xexio/Downloads/torrent/archlinux-2018.06.01-x86_64.iso";

    qDebug() << "Command " <<qemuCommand;

    return qemuCommand;

}

bool MachineUtils::deleteMachine(const QUuid machineUuid) {

    // TODO: Change all of this code to SystemUtils::getMachinePath
    QSettings settings;
    settings.beginGroup("DataFolder");

    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all the machines included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonArray machines = machinesDocument["machines"].toArray();

    int machinePos = 0;
    bool machineExists = false;
    QString machinePath;
    while(machinePos < machines.size() && ! machineExists) {
        QJsonObject machineJSON = machines[machinePos].toObject();

        if (machineUuid == machineJSON["uuid"].toVariant()) {
            machineExists = true;
            machinePath = machineJSON["path"].toString();
        } else {
            ++machinePos;
        }
    }

    if (machineExists) {
        machines.removeAt(machinePos);
    }

    QJsonObject machinesObject;
    machinesObject["machines"] = machines;
    QJsonDocument machinesDocumentJSON(machinesObject);

    machinesFile.seek(0);
    machinesFile.resize(0);
    machinesFile.write(machinesDocumentJSON.toJson());
    machinesFile.close();

    QDir *machineDirectory = new QDir(QDir::toNativeSeparators(machinePath));

    bool removedDirectory = machineDirectory -> removeRecursively();

    return removedDirectory;
}

QJsonObject MachineUtils::getMachineJsonObject(const QUuid machineUuid) {

    QString machineConfigPath = getMachineConfigPath(machineUuid);

    QFile machineFile(machineConfigPath);
    machineFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all data included in the file
    QByteArray machineData = machineFile.readAll();
    QJsonDocument machineDocument(QJsonDocument::fromJson(machineData));
    QJsonObject machineObject = machineDocument.object();

    return machineObject;
}

QString MachineUtils::getMachinePath(const QUuid machineUuid) {

    QSettings settings;
    settings.beginGroup("DataFolder");

    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all the machines included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonArray machines = machinesDocument["machines"].toArray();

    int machinePos = 0;
    bool machineExists = false;
    QString machinePath;
    while(machinePos < machines.size() && ! machineExists) {
        QJsonObject machineJSON = machines[machinePos].toObject();

        if (machineUuid == machineJSON["uuid"].toVariant()) {
            machineExists = true;
            machinePath = machineJSON["path"].toString();
        } else {
            ++machinePos;
        }
    }

    return machinePath;
}

QString MachineUtils::getMachineConfigPath(const QUuid machineUuid) {

    QSettings settings;
    settings.beginGroup("DataFolder");

    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all the machines included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonArray machines = machinesDocument["machines"].toArray();

    int machinePos = 0;
    bool machineExists = false;
    QString machinePath;
    while(machinePos < machines.size() && ! machineExists) {
        QJsonObject machineJSON = machines[machinePos].toObject();

        if (machineUuid == machineJSON["uuid"].toVariant()) {
            machineExists = true;
            machinePath = machineJSON["configpath"].toString();
        } else {
            ++machinePos;
        }
    }

    return machinePath;
}

QStringList MachineUtils::getSoundCards(QJsonArray soundCardsArray) {

    QStringList soundCardsList;
    for(int i = 0; i < soundCardsArray.size(); ++i) {
        soundCardsList.append(soundCardsArray[i].toString());
    }

    return soundCardsList;
}

QStringList MachineUtils::getAccelerators(QJsonArray acceleratorsArray) {

    QStringList acceleratorsList;
    for(int i = 0; i < acceleratorsArray.size(); ++i) {
        acceleratorsList.append(acceleratorsArray[i].toString());
    }

    return acceleratorsList;
}

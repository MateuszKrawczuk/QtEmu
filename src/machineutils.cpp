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

MachineUtils::MachineUtils(QObject *parent) : QObject(parent)
{
    qDebug() << "MachineUtils object created";
}

MachineUtils::~MachineUtils()
{
    qDebug() << "MachineUtils object destroyed";
}

/**
 * @brief Delete the machine
 * @param machineUuid, uuid of the machine
 * @return true if the machine is deleted
 *
 * Delete all the files of the machine
 */
bool MachineUtils::deleteMachine(const QUuid machineUuid)
{
    QSettings settings;
    settings.beginGroup("DataFolder");
    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");
    QFile machinesFile(qtemuConfig);
    if (!machinesFile.open(QFile::ReadWrite)) {
        QMessageBox *m_deleteMachineMessageBox = new QMessageBox();
        m_deleteMachineMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_deleteMachineMessageBox->setIcon(QMessageBox::Critical);
        m_deleteMachineMessageBox->setText(tr("<p>Cannot delete the machine</p>"
                                              "<p>The file with the machine configuration are not readable</p>"));
        m_deleteMachineMessageBox->exec();
        return false;
    }

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
    bool removedDirectory = machineDirectory->removeRecursively();

    return removedDirectory;
}

/**
 * @brief Get the machine json object
 * @param machineUuid, uuid of the machine
 * @return machine json object
 *
 * Get the machine json object
 */
QJsonObject MachineUtils::getMachineJsonObject(const QUuid machineUuid)
{
    QString machineConfigPath = getMachineConfigPath(machineUuid);

    if (machineConfigPath.isEmpty()) {
        // TODO...
    }

    QFile machineFile(machineConfigPath);
    machineFile.open(QIODevice::ReadWrite);

    // Read all data included in the file
    QByteArray machineData = machineFile.readAll();
    QJsonDocument machineDocument(QJsonDocument::fromJson(machineData));
    QJsonObject machineObject = machineDocument.object();

    return machineObject;
}

/**
 * @brief Get the machine path
 * @param machineUuid, uuid of the machine
 * @return path where the machine is located
 *
 * Get the machine path
 */
QString MachineUtils::getMachineConfigPath(const QUuid machineUuid)
{
    QSettings settings;
    settings.beginGroup("DataFolder");
    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");
    QFile machinesFile(qtemuConfig);
    if (!machinesFile.open(QFile::ReadOnly)) {
        QMessageBox *m_machinePathMessageBox = new QMessageBox();
        m_machinePathMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_machinePathMessageBox->setIcon(QMessageBox::Critical);
        m_machinePathMessageBox->setText(tr("<p>Cannot open the file</p>"
                                            "<p>The file with the machine configuration of the machine are not readable</p>"));
        m_machinePathMessageBox->exec();
        return "";
    }

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

/**
 * @brief Get the sound cards
 * @param soundCardsArray, json array with the sound cards of the machine
 * @return list with the sound cards
 *
 * Get the sound cards
 */
QStringList MachineUtils::getSoundCards(QJsonArray soundCardsArray)
{
    QStringList soundCardsList;
    for(int i = 0; i < soundCardsArray.size(); ++i) {
        soundCardsList.append(soundCardsArray[i].toString());
    }

    return soundCardsList;
}

/**
 * @brief Get the accelerators
 * @param acceleratorsArray, json array with the accelerators of the machine
 * @return list with the accelerators
 *
 * Get the accelerators
 */
QStringList MachineUtils::getAccelerators(QJsonArray acceleratorsArray)
{
    QStringList acceleratorsList;
    for(int i = 0; i < acceleratorsArray.size(); ++i) {
        acceleratorsList.append(acceleratorsArray[i].toString());
    }

    return acceleratorsList;
}

/**
 * @brief Get the media devices
 * @param mediaDevicesArray, json array with the media devices of the machine
 * @return list with the media devices
 *
 * Get the media devices
 */
QStringList MachineUtils::getMediaDevices(QJsonArray mediaDevicesArray)
{
    QStringList mediaDevicesList;
    for(int i= 0; i < mediaDevicesArray.size(); ++i) {
        mediaDevicesList.append(mediaDevicesArray[i].toString());
    }

    return mediaDevicesList;
}

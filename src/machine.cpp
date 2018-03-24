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

// Qt

// Local
#include "machine.h"

Machine::Machine(QObject *parent) : QObject(parent) {
    qDebug() << "Machine object created";
}

Machine::~Machine() {
    qDebug() << "Machine object destroyed";
}

QString Machine::getName() const {
    return name;
}

void Machine::setName(const QString &value) {
    name = value;
}

QString Machine::getOSType() const {
    return OSType;
}

void Machine::setOSType(const QString &value) {
    OSType = value;
}

QString Machine::getOSVersion() const {
    return OSVersion;
}

void Machine::setOSVersion(const QString &value) {
    OSVersion = value;
}

QString Machine::getCPUType() const {
    return CPUType;
}

void Machine::setCPUType(const QString &value) {
    CPUType = value;
}

ushort Machine::getCPUCount() const {
    return CPUCount;
}

void Machine::setCPUCount(const ushort &value) {
    CPUCount = value;
}

ushort Machine::getSocketCount() const {
    return socketCount;
}

void Machine::setSocketCount(const ushort &value) {
    socketCount = value;
}

ushort Machine::getCoresSocket() const {
    return coresSocket;
}

void Machine::setCoresSocket(const ushort &value) {
    coresSocket = value;
}

ushort Machine::getThreadsCore() const {
    return threadsCore;
}

void Machine::setThreadsCore(const ushort &value) {
    threadsCore = value;
}

ushort Machine::getMaxHotCPU() const {
    return maxHotCPU;
}

void Machine::setMaxHotCPU(const ushort &value) {
    maxHotCPU = value;
}

QString Machine::getGPUType() const {
    return GPUType;
}

void Machine::setGPUType(const QString &value) {
    GPUType = value;
}

QString Machine::getKeyboard() const {
    return keyboard;
}

void Machine::setKeyboard(const QString &value) {
    keyboard = value;
}

qlonglong Machine::getRAM() const {
    return RAM;
}

void Machine::setRAM(const qlonglong &value) {
    RAM = value;
}

QHash<QString, QString> Machine::getAudio() const {
    return audio;
}

void Machine::setAudio(const QHash<QString, QString> &value) {
    audio = value;
}

bool Machine::getUseNetwork() const {
    return useNetwork;
}

void Machine::setUseNetwork(bool value) {
    useNetwork = value;
}

QString Machine::getDiskName() const {
    return diskName;
}

void Machine::setDiskName(const QString &value) {
    diskName = value;
}

QString Machine::getDiskPath() const {
    return diskPath;
}

void Machine::setDiskPath(const QString &value) {
    diskPath = value;
}

qlonglong Machine::getDiskSize() const {
    return diskSize;
}

void Machine::setDiskSize(const qlonglong &value) {
    diskSize = value;
}

QString Machine::getDiskFormat() const {
    return diskFormat;
}

void Machine::setDiskFormat(const QString &value) {
    diskFormat = value;
}

bool Machine::getCreateNewDisk() const {
    return createNewDisk;
}

void Machine::setCreateNewDisk(bool value) {
    createNewDisk = value;
}

QHash<QString, QString> Machine::getAccelerator() const {
    return accelerator;
}

void Machine::setAccelerator(const QHash<QString, QString> &value) {
    accelerator = value;
}

void Machine::addAudio(const QString key, const QString value) {
    this -> audio.insert(key, value);
}

void Machine::removeAudio(const QString audio) {
    if(this -> audio.contains(audio)){
        this -> audio.remove(audio);
    }
}

void Machine::addAccelerator(const QString key, const QString value) {
    this -> accelerator.insert(key, value);
}

void Machine::removeAccelerator(const QString accelerator) {
    if(this -> accelerator.contains(accelerator)){
        this -> accelerator.remove(accelerator);
    }
}

QString Machine::getAudioLabel() {
    QHash<QString, QString>::const_iterator i = audio.constBegin();
    QString audioLabel;
    bool first = true;

    while (i != audio.constEnd()) {
        if(first){
            first = false;
        } else {
            audioLabel.append(", ");
        }

        audioLabel.append(i.value());
        ++i;
    }

    return audioLabel;

}

QString Machine::getAcceleratorLabel() {
    QHash<QString, QString>::const_iterator i = accelerator.constBegin();
    QString acceleratorLabel;
    bool first = true;

    while (i != accelerator.constEnd()) {
        if(first){
            first = false;
        } else {
            acceleratorLabel.append(", ");
        }

        acceleratorLabel.append(i.value().toUpper());
        ++i;
    }

    return acceleratorLabel;
}

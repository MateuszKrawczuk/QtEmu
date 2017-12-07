/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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

unsigned short Machine::getCPUCount() const {
    return CPUCount;
}

void Machine::setCPUCount(unsigned short value) {
    CPUCount = value;
}

unsigned short Machine::getSocketCount() const {
    return socketCount;
}

void Machine::setSocketCount(unsigned short value) {
    socketCount = value;
}

unsigned short Machine::getCoresSocket() const {
    return coresSocket;
}

void Machine::setCoresSocket(unsigned short value) {
    coresSocket = value;
}

unsigned short Machine::getThreadsCore() const {
    return threadsCore;
}

void Machine::setThreadsCore(unsigned short value) {
    threadsCore = value;
}

unsigned short Machine::getMaxHotCPU() const {
    return maxHotCPU;
}

void Machine::setMaxHotCPU(unsigned short value) {
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

unsigned long long Machine::getRAM() const {
    return RAM;
}

void Machine::setRAM(unsigned long long value) {
    RAM = value;
}

QHash<QString, QString> Machine::getAudio() const {
    return audio;
}

bool Machine::getUseNetwork() const {
    return useNetwork;
}

void Machine::setUseNetwork(bool value) {
    useNetwork = value;
}

QString Machine::getDiskPath() const {
    return diskPath;
}

void Machine::setDiskPath(const QString &value) {
    diskPath = value;
}

QHash<QString, QString> Machine::getAccelerator() const {
    return accelerator;
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

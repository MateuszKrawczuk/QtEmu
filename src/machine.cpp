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
    this -> m_machineProcess = new QProcess(this);

    connect(m_machineProcess, &QProcess::readyReadStandardOutput,
            this, &Machine::readMachineStandardOut);

    connect(m_machineProcess, &QProcess::readyReadStandardError,
            this, &Machine::readMachineErrorOut);

    connect(m_machineProcess, &QProcess::started,
            this, &Machine::machineStarted);

    connect(m_machineProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Machine::machineFinished);

    qDebug() << "Machine object created";
}

Machine::~Machine() {
    qDebug() << "Machine object destroyed";
}

/**
 * @brief Get the name of the machine
 *
 * Get the name of the machine
 */
QString Machine::getName() const {
    return name;
}

/**
 * @brief Set the name of the machine
 *
 * Set the name of the machine
 */
void Machine::setName(const QString &value) {
    name = value;
}

/**
 * @brief Get the operating system type of the machine
 *
 * Get the operating system type of the machine.
 * Ex: GNU/Linux, Windows...
 */
QString Machine::getOSType() const {
    return OSType;
}

/**
 * @brief Set the operating system type of the machine
 *
 * Set the operating system type of the machine.
 * Ex: GNU/Linux, Windows...
 */
void Machine::setOSType(const QString &value) {
    OSType = value;
}

/**
 * @brief Get the operating system version of the machine
 *
 * Get the operating system version of the machine.
 * Ex: Debian, Mageia, Arch Linux...
 */
QString Machine::getOSVersion() const {
    return OSVersion;
}

/**
 * @brief Set the operating system version of the machine
 *
 * Set the operating system version of the machine.
 * Ex: Debian, Mageia, Arch Linux...
 */
void Machine::setOSVersion(const QString &value) {
    OSVersion = value;
}

/**
 * @brief Get the machine path
 *
 * Get the machine path
 * Ex: /home/xexio/Vms/Debian
 */
QString Machine::getPath() const {
    return path;
}

/**
 * @brief Set the machine path
 *
 * Set the machine path
 * Ex: /home/xexio/Vms/Debian
 */
void Machine::setPath(const QString &value) {
    path = value;
}

/**
 * @brief Get the configuration path
 *
 * Get the configuration path
 */
QString Machine::getConfigPath() const {
    return configPath;
}

/**
 * @brief Set the configuration path
 *
 * Set the configuration path
 */
void Machine::setConfigPath(const QString &value) {
    configPath = value;
}

/**
 * @brief Get the machine uuid
 *
 * get the machine uuid with a unique key generated
 * with QUuid
 * Ex: {fc6a2dd5-3c31-401f-a9c7-86ad6190a77f}
 */
QString Machine::getUuid() const {
    return uuid;
}

/**
 * @brief Set the machine uuid
 *
 * Set the machine uuid with a unique key generated
 * with QUuid
 * Ex: {fc6a2dd5-3c31-401f-a9c7-86ad6190a77f}
 */
void Machine::setUuid(const QString &value) {
    uuid = value;
}

/**
 * @brief Get the machine state
 *
 * Get the machine state
 * Ex: Started, Stopped or Rebooted
 */
Machine::States Machine::getState() const {
    return state;
}

/**
 * @brief Set the machine state
 *
 * Set the machine state
 * Ex: Started, Stopped or Rebooted
 */
void Machine::setState(const States &value) {
    state = value;
}

/**
 * @brief Get the CPU Type of the machine
 *
 * Get the CPU Type of the machine
 * Ex: core2duo, pentium, Opteron_G5...
 */
QString Machine::getCPUType() const {
    return CPUType;
}

/**
 * @brief Set the CPU Type of the machine
 *
 * Set the CPU Type of the machine
 * Ex: core2duo, pentium, Opteron_G5...
 */
void Machine::setCPUType(const QString &value) {
    CPUType = value;
}

/**
 * @brief Get the CPU Count of the machine
 *
 * Get the CPU Count of the machine
 */
qint64 Machine::getCPUCount() const {
    return CPUCount;
}

/**
 * @brief Set the CPU Count of the machine
 *
 * Set the CPU Count of the machine
 */
void Machine::setCPUCount(const qint64 &value) {
    CPUCount = value;
}

/**
 * @brief Get the CPU Socket Count of the machine
 *
 * Get the CPU Socket Count of the machine
 */
qint64 Machine::getSocketCount() const {
    return socketCount;
}

/**
 * @brief Set the CPU Socket Count of the machine
 *
 * Set the CPU Socket Count of the machine
 */
void Machine::setSocketCount(const qint64 &value) {
    socketCount = value;
}

/**
 * @brief Get the CPU Cores per Socket of the machine
 *
 * Get the CPU Cores per Socket of the machine
 */
qint64 Machine::getCoresSocket() const {
    return coresSocket;
}

/**
 * @brief Set the CPU Cores per Socket of the machine
 *
 * Set the CPU Cores per Socket of the machine
 */
void Machine::setCoresSocket(const qint64 &value) {
    coresSocket = value;
}

/**
 * @brief Get the CPU Threads per Core of the machine
 *
 * Get the CPU Threads per Core of the machine
 */
qint64 Machine::getThreadsCore() const {
    return threadsCore;
}

/**
 * @brief Set the CPU Threads per Core of the machine
 *
 * Set the CPU Threads per Core of the machine
 */
void Machine::setThreadsCore(const qint64 &value) {
    threadsCore = value;
}

/**
 * @brief Get the max hot CPUs of the machine
 *
 * Get the max hot CPUs of the machine
 */
qint64 Machine::getMaxHotCPU() const {
    return maxHotCPU;
}

/**
 * @brief Set the max hot CPUs of the machine
 *
 * Set the max hot CPUs of the machine
 */
void Machine::setMaxHotCPU(const qint64 &value) {
    maxHotCPU = value;
}

/**
 * @brief Get the GPU of the machine
 *
 * Get the GPU of the machine
 * Ex: std, vmware, cg3...
 */
QString Machine::getGPUType() const {
    return GPUType;
}

/**
 * @brief Set the GPU of the machine
 *
 * Set the GPU of the machine
 * Ex: std, vmware, cg3...
 */
void Machine::setGPUType(const QString &value) {
    GPUType = value;
}

/**
 * @brief Get the keyboard of the machine
 *
 * Get the keyboard of the machine
 * Ex: ar, es, hr...
 */
QString Machine::getKeyboard() const {
    return keyboard;
}

/**
 * @brief Set the keyboard of the machine
 *
 * Set the keyboard of the machine
 * Ex: ar, es, hr...
 */
void Machine::setKeyboard(const QString &value) {
    keyboard = value;
}

/**
 * @brief Get the RAM of the machine
 *
 * Get the RAM of the machine.
 * There's a limit in the RAM that the machine can use.
 * You can't use more RAM that your system have.
 *
 * Ex: If your computer have 8GiB of RAM, your machine
 * are limited to that amount of RAM
 */
qlonglong Machine::getRAM() const {
    return RAM;
}

/**
 * @brief Set the RAM of the machine
 *
 * Set the RAM of the machine.
 * There's a limit in the RAM that the machine can use.
 * You can't use more RAM that your system have.
 *
 * Ex: If your computer have 8GiB of RAM, your machine
 * are limited to that amount of RAM
 */
void Machine::setRAM(const qlonglong &value) {
    RAM = value;
}

/**
 * @brief Get the audio cards of the machine
 *
 * Get the audio cards of the machine
 * Ex: ac97, es1370, hda...
 */
QHash<QString, QString> Machine::getAudio() const {
    return audio;
}

/**
 * @brief Set the audio cards of the machine
 *
 * Set the audio cards of the machine
 * Ex: ac97, es1370, hda...
 */
void Machine::setAudio(const QHash<QString, QString> &value) {
    audio = value;
}

/**
 * @brief Get if the machine have network
 *
 * Get if the machine have network
 */
bool Machine::getUseNetwork() const {
    return useNetwork;
}

/**
 * @brief Set if the machine have network
 *
 * Set if the machine have network
 */
void Machine::setUseNetwork(bool value) {
    useNetwork = value;
}

/**
 * @brief Get the list of media
 * @return media list
 *
 * Get the list of media of the machine
 */
QList<Media> Machine::getMedia() const {
    return media;
}

/**
 * @brief Add media to the media list
 * @param value, new media
 *
 * Add media to the media list
 */
void Machine::addMedia(const Media value) {
    this -> media.append(value);
}

/**
 * @brief Get if there's necessary create a disk for the machine
 *
 * Get if there's necessary create a disk for the machine
 */
bool Machine::getCreateNewDisk() const {
    return createNewDisk;
}

/**
 * @brief Set if there's necessary create a disk for the machine
 *
 * Set if there's necessary create a disk for the machine
 */
void Machine::setCreateNewDisk(bool value) {
    createNewDisk = value;
}

/**
 * @brief Get the disk format
 *
 * Get the disk format
 * Ex: qcow2, vmdk, raw...
 */
QString Machine::getDiskFormat() const {
    return diskFormat;
}

/**
 * @brief Get the disk format
 *
 * Get the disk format
 * Ex: qcow2, vmdk, raw...
 */
void Machine::setDiskFormat(const QString &value) {
    diskFormat = value;
}

/**
 * @brief Get the disk size
 *
 * Get the disk size
 */
qlonglong Machine::getDiskSize() const {
    return diskSize;
}

/**
 * @brief Set the disk size
 *
 * Set the disk size
 */
void Machine::setDiskSize(const qlonglong &value) {
    diskSize = value;
}

/**
 * @brief Get the disk path of the machine
 *
 * Get the disk path of the machine
 * Ex: /home/xexio/Vms/Debian/debian.qcow2
 */
QString Machine::getDiskPath() const {
    return diskPath;
}

/**
 * @brief Set the disk path of the machine
 *
 * Set the disk path of the machine
 * Ex: /home/xexio/Vms/Debian/debian.qcow2
 */
void Machine::setDiskPath(const QString &value) {
    diskPath = value;
}

/**
 * @brief Get the disk name of the machine
 * Get the disk name of the machine
 * Ex: debian
 */
QString Machine::getDiskName() const {
    return diskName;
}

/**
 * @brief Set the disk name of the machine
 * Set the disk name of the machine
 * Ex: debian
 */
void Machine::setDiskName(const QString &value) {
    diskName = value;
}

/**
 * @brief Get the accelerator machine
 *
 * Get the accelerator machine
 * Ex: kvm, xen...
 */
QHash<QString, QString> Machine::getAccelerator() const {
    return accelerator;
}

/**
 * @brief Set the accelerator machine
 *
 * Set the accelerator machine
 * Ex: kvm, xen...
 */
void Machine::setAccelerator(const QHash<QString, QString> &value) {
    accelerator = value;
}

// Methods
void Machine::addAudio(const QString key, const QString value) {
    this -> audio.insert(key, value);
}

/**
 * @brief Remove the audio card from the list
 * @param audio, audio card to be removed
 *
 * Remove the audio card from the list
 */
void Machine::removeAudio(const QString audio) {
    if(this -> audio.contains(audio)){
        this -> audio.remove(audio);
    }
}

/**
 * @brief Add an accelerator to the map
 * @param key code of the accelerator
 * @param value description of the accelerator
 *
 * Add an accelerator to the map
 */
void Machine::addAccelerator(const QString key, const QString value) {
    this -> accelerator.insert(key, value);
}

/**
 * @brief Remove an accelerator from the map
 * @param key code of the accelerator
 *
 * Remove an accelerator from the map if exists
 */
void Machine::removeAccelerator(const QString accelerator) {
    if(this -> accelerator.contains(accelerator)){
        this -> accelerator.remove(accelerator);
    }
}

/**
 * @brief Get all the audio cards separated by commas
 * @param showKey, return the key or the value
 * @return Audio cards separated by commas
 *
 * Get all the audio cards separated by commas
 * Ex: ac97, es1370, hda
 */
QString Machine::getAudioLabel(const bool showKey) {
    QHash<QString, QString>::const_iterator i = audio.constBegin();
    QString audioLabel;
    bool first = true;

    while (i != audio.constEnd()) {
        if(first){
            first = false;
        } else {
            audioLabel.append(", ");
        }

        if (showKey) {
            audioLabel.append(i.key());
        } else {
            audioLabel.append(i.value());
        }
        ++i;
    }

    return audioLabel;

}

/**
 * @brief Get all the accelerators separated by commas
 * @param showKey, return the key or the value
 * @return Accelerators separated by commas
 *
 * Get all the accelerators separated by commas
 * Ex: kvm,tcg
 */
QString Machine::getAcceleratorLabel(const bool showKey) {
    QHash<QString, QString>::const_iterator i = accelerator.constBegin();
    QString acceleratorLabel;
    bool first = true;

    while (i != accelerator.constEnd()) {
        if(first){
            first = false;
        } else {
            acceleratorLabel.append(", ");
        }

        if (showKey) {
            acceleratorLabel.append(i.key());
        } else {
            acceleratorLabel.append(i.value());
        }

        ++i;
    }

    return acceleratorLabel;
}

void Machine::runMachine(const QUuid machineUuid) {

    QStringList args = MachineUtils::generateMachineCommand(machineUuid);

    QString program;

    #ifdef Q_OS_LINUX
    program = "qemu-system-x86_64";
    #endif
    #ifdef Q_OS_WIN
    // TODO: Control windows execution
    #endif
    #ifdef Q_OS_MACOS
    // TODO: Control MacOS execution
    #endif

    // TODO
    //m_machineProcess -> setProcessEnvironment(buildEnvironment());

    m_machineProcess -> start(program, args);
}

void Machine::stopMachine() {
    this -> m_machineProcess -> write(qPrintable("system_powerdown\n"));
    this -> state = Machine::Stopped;

    emit(machineStateChangedSignal(Machine::Stopped));
}

void Machine::resetMachine() {
    this -> m_machineProcess -> write(qPrintable("system_reset\n"));
}

void Machine::pauseMachine() {

    if (state == Machine::Started) {
        this -> m_machineProcess -> write(qPrintable("stop\n"));
        this -> state = Machine::Paused;

        emit(machineStateChangedSignal(Machine::Paused));

    } else if (state == Machine::Paused) {
        this -> m_machineProcess -> write(qPrintable("cont\n"));
        this -> state = Machine::Started;

        emit(machineStateChangedSignal(Machine::Started));
    }
}

void Machine::readMachineStandardOut() {
    // TODO: Show in a window
    qDebug() << "Standard Out: " << this -> m_machineProcess -> readAllStandardOutput();
}

void Machine::readMachineErrorOut() {
    // TODO: Show in a window
    qDebug() << "Error Out: " <<  m_machineProcess -> readAllStandardError();
}

void Machine::machineStarted() {
    this -> state = Machine::Started;

    emit(machineStateChangedSignal(Machine::Started));
}

void Machine::machineFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    qDebug() << "Exit code: " << exitCode << " exit status: " << exitStatus;
    this -> state = Machine::Stopped;
    emit(machineStateChangedSignal(Machine::Stopped));
}

QString Machine::getDescription() const
{
    return description;
}

void Machine::setDescription(const QString &value)
{
    description = value;
}

QProcessEnvironment Machine::buildEnvironment() {
    
    // TODO: Implement Windows and MacOS
    QProcessEnvironment env = m_machineProcess -> processEnvironment();
    env.insert("QEMU_AUDIO_DRV", "alsa");

    return env;
}

Media::Media() {
    qDebug() << "Media object created";
}

Media::~Media() {
    qDebug() << "Media object destroyed";
}

/**
 * @brief Get the media name
 * @return media name
 *
 * Get the media names
 */
QString Media::mediaName() const {
    return m_mediaName;
}

/**
 * @brief Set the media name
 * @param mediaName, new name for the media
 *
 * Set the media name
 */
void Media::setMediaName(const QString &mediaName) {
    m_mediaName = mediaName;
}

/**
 * @brief Get the media path
 * @return media path
 *
 * Get the media path
 */
QString Media::mediaPath() const {
    return m_mediaPath;
}

/**
 * @brief Set the new media path
 * @param mediaPath, new media path
 *
 * Set the new media path
 */
void Media::setMediaPath(const QString &mediaPath) {
    m_mediaPath = mediaPath;
}

/**
 * @brief Get the media size
 * @return media size
 *
 * Get the media size
 */
qlonglong Media::mediaSize() const {
    return m_mediaSize;
}

/**
 * @brief Set the media size
 * @param mediaSize, media size
 *
 * Set the media size
 */
void Media::setMediaSize(const qlonglong &mediaSize) {
    m_mediaSize = mediaSize;
}

/**
 * @brief Get the media type
 * @return media type
 *
 * Get the media type
 */
QString Media::mediaType() const {
    return m_mediaType;
}

/**
 * @brief Set the media type
 * @param mediaType, new media type
 *
 * Set the media type
 * Ex: ide, scsi...
 */
void Media::setMediaType(const QString &mediaType) {
    m_mediaType = mediaType;
}

/**
 * @brief Get the media format
 * @return media format
 *
 * Get the media format
 * Ex: qcow, qcow2, raw...
 */
QString Media::mediaFormat() const {
    return m_mediaFormat;
}

/**
 * @brief Set the media format
 * @param mediaFormat, new media format
 *
 * Set the media format
 */
void Media::setMediaFormat(const QString &mediaFormat) {
    m_mediaFormat = mediaFormat;
}

/**
 * @brief Get the media interface
 * @return media interface
 *
 * Get the media interface
 * Ex: ide, scsi...
 */
QString Media::mediaInterface() const {
    return m_mediaInterface;
}

/**
 * @brief Set the new media interface
 * @param mediaInterface, media interface
 *
 * Set the new media interface
 */
void Media::setMediaInterface(const QString &mediaInterface) {
    m_mediaInterface = mediaInterface;
}

/**
 * @brief Get the media cache
 * @return the media cache
 *
 * Get the media cache
 * Ex: none, writeback...
 */
QString Media::mediaCache() const {
    return m_mediaCache;
}

/**
 * @brief Set the media cache
 * @param mediaCache, new media cache
 *
 * Set the media cache
 */
void Media::setMediaCache(const QString &mediaCache) {
    m_mediaCache = mediaCache;
}

/**
 * @brief Get the media IO
 * @return media IO
 *
 * Get the media IO
 * Ex: threads, native...
 */
QString Media::mediaIO() const {
    return m_mediaIO;
}

/**
 * @brief Set the media IO
 * @param mediaIO, new media IO
 *
 * Set the media IO
 */
void Media::setMediaIO(const QString &mediaIO) {
    m_mediaIO = mediaIO;
}

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

#ifndef MACHINE_H
#define MACHINE_H

// Qt
#include <QObject>
#include <QProcess>
#include <QHash>
#include <QUuid>

#include <QDebug>

// Local
#include "machineutils.h"

class Boot {

    public:
        explicit Boot();
        ~Boot();

        bool bootMenu() const;
        void setBootMenu(bool bootMenu);

        bool kernelBootEnabled() const;
        void setKernelBootEnabled(bool kernelBootEnabled);

        QString kernelPath() const;
        void setKernelPath(const QString &kernelPath);

        QString initrdPath() const;
        void setInitrdPath(const QString &initrdPath);

        QString kernelArgs() const;
        void setKernelArgs(const QString &kernelArgs);

        QMap<int, QString> getBootOrder() const;
        void setBootOrder(const QMap<int, QString> &value);

    signals:

    public slots:

    private slots:

    protected:

    private:
        bool m_bootMenu;
        bool m_kernelBootEnabled;
        QString m_kernelPath;
        QString m_initrdPath;
        QString m_kernelArgs;
        QMap<int, QString> bootOrder;

};

class Media {

    public:
        explicit Media();
        ~Media();

        QString mediaName() const;
        void setMediaName(const QString &mediaName);

        QString mediaPath() const;
        void setMediaPath(const QString &mediaPath);

        qlonglong mediaSize() const;
        void setMediaSize(const qlonglong &mediaSize);

        QString mediaType() const;
        void setMediaType(const QString &mediaType);

        QString mediaFormat() const;
        void setMediaFormat(const QString &mediaFormat);

        QString mediaInterface() const;
        void setMediaInterface(const QString &mediaInterface);

        QString mediaCache() const;
        void setMediaCache(const QString &mediaCache);

        QString mediaIO() const;
        void setMediaIO(const QString &mediaIO);

    signals:

    public slots:

    private slots:

    protected:

    private:
        QString m_mediaName;
        QString m_mediaPath;
        qlonglong m_mediaSize;
        QString m_mediaType;
        QString m_mediaFormat;
        QString m_mediaInterface;
        QString m_mediaCache;
        QString m_mediaIO;
};

class Machine: public QObject {
    Q_OBJECT

    public:
        explicit Machine(QObject *parent = 0);
        ~Machine();

        enum States {
            Started, Stopped, Saved, Paused
        };

        QString getName() const;
        void setName(const QString &value);

        QString getOSType() const;
        void setOSType(const QString &value);

        QString getOSVersion() const;
        void setOSVersion(const QString &value);

        QString getPath() const;
        void setPath(const QString &value);

        QString getConfigPath() const;
        void setConfigPath(const QString &value);

        QString getUuid() const;
        void setUuid(const QString &value);

        QString getDescription() const;
        void setDescription(const QString &value);

        Machine::States getState() const;
        void setState(const States &value);

        QString getCPUType() const;
        void setCPUType(const QString &value);

        qint64 getCPUCount() const;
        void setCPUCount(const qint64 &value);

        qint64 getSocketCount() const;
        void setSocketCount(const qint64 &value);

        qint64 getCoresSocket() const;
        void setCoresSocket(const qint64 &value);

        qint64 getThreadsCore() const;
        void setThreadsCore(const qint64 &value);

        qint64 getMaxHotCPU() const;
        void setMaxHotCPU(const qint64 &value);

        QString getGPUType() const;
        void setGPUType(const QString &value);

        QString getKeyboard() const;
        void setKeyboard(const QString &value);

        qlonglong getRAM() const;
        void setRAM(const qlonglong &value);

        QStringList getAudio() const;
        void setAudio(const QStringList &value);

        bool getUseNetwork() const;
        void setUseNetwork(bool value);

        QList<Media> getMedia() const;
        void addMedia(const Media value);

        QString getDiskName() const;
        void setDiskName(const QString &value);

        QString getDiskPath() const;
        void setDiskPath(const QString &value);

        qlonglong getDiskSize() const;
        void setDiskSize(const qlonglong &value);

        QString getDiskFormat() const;
        void setDiskFormat(const QString &value);

        bool getCreateNewDisk() const;
        void setCreateNewDisk(bool value);

        QStringList getAccelerator() const;
        void setAccelerator(const QStringList &value);

        Boot getMachineBoot() const;
        void setMachineBoot(const Boot &machineBoot);

        // Methods
        void addAudio(const QString audio);
        void removeAudio(const QString audio);

        void addAccelerator(const QString accel);
        void removeAccelerator(const QString accel);

        QString getAudioLabel();
        QString getAcceleratorLabel();

        void runMachine(const QUuid machineUuid);
        void stopMachine();
        void resetMachine();
        void pauseMachine();

    signals:
        void machineStateChangedSignal(States newState);

    public slots:

    private slots:
        void readMachineStandardOut();
        void readMachineErrorOut();
        void machineStarted();
        void machineFinished(int exitCode, QProcess::ExitStatus exitStatus);

    protected:

    private:
        // General
        QString name;
        QString OSType;
        QString OSVersion;
        QString path;
        QString configPath;
        QString uuid;
        QString description;
        States state;

        // Hardware - CPU
        QString CPUType;
        qint64 CPUCount;
        qint64 socketCount;
        qint64 coresSocket;
        qint64 threadsCore;
        qint64 maxHotCPU;

        // Hardware - GPU
        QString GPUType;
        QString keyboard;

        // Hardware - RAM
        qlonglong RAM;

        // Hardware - Audio
        QStringList audio;

        // Hardware - Network
        bool useNetwork;

        // Hardware - media
        QList<Media> media;

        // Hardware - machine wizard
        QString diskName;
        QString diskPath;
        qlonglong diskSize;
        QString diskFormat;
        bool createNewDisk;

        // Accelerator
        QStringList accelerator;

        // Boot
        Boot m_machineBoot;

        // Process
        QProcess *m_machineProcess;

        // Methods
        QProcessEnvironment buildEnvironment();
};

#endif // MACHINE_H

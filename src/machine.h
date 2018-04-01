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
#include <QHash>

#include <QDebug>

// Local

class Machine: public QObject {
    Q_OBJECT

    public:
        explicit Machine(QObject *parent = 0);
        ~Machine();

        enum States {
            Started, Stopped, Rebooted
        };

        QString getName() const;
        void setName(const QString &value);

        QString getOSType() const;
        void setOSType(const QString &value);

        QString getOSVersion() const;
        void setOSVersion(const QString &value);

        QString getCPUType() const;
        void setCPUType(const QString &value);

        ushort getCPUCount() const;
        void setCPUCount(const ushort &value);

        ushort getSocketCount() const;
        void setSocketCount(const ushort &value);

        ushort getCoresSocket() const;
        void setCoresSocket(const ushort &value);

        ushort getThreadsCore() const;
        void setThreadsCore(const ushort &value);

        ushort getMaxHotCPU() const;
        void setMaxHotCPU(const ushort &value);

        QString getGPUType() const;
        void setGPUType(const QString &value);

        QString getKeyboard() const;
        void setKeyboard(const QString &value);

        qlonglong getRAM() const;
        void setRAM(const qlonglong &value);

        QHash<QString, QString> getAudio() const;
        void setAudio(const QHash<QString, QString> &value);

        bool getUseNetwork() const;
        void setUseNetwork(bool value);

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

        QHash<QString, QString> getAccelerator() const;
        void setAccelerator(const QHash<QString, QString> &value);

        void addAudio(const QString key, const QString value);
        void removeAudio(const QString key);

        void addAccelerator(const QString key, const QString value);
        void removeAccelerator(const QString accelerator);

        QString getAudioLabel();

        QString getAcceleratorLabel();

    signals:

    public slots:

    protected:

    private:
        // General
        QString name;
        QString OSType;
        QString OSVersion;

        // Hardware - CPU
        QString CPUType;
        ushort CPUCount;
        ushort socketCount;
        ushort coresSocket;
        ushort threadsCore;
        ushort maxHotCPU;

        // Hardware - GPU
        QString GPUType;
        QString keyboard;

        // Hardware - RAM
        qlonglong RAM;

        // Hardware - Audio
        QHash<QString, QString> audio;

        // Hardware - Network
        bool useNetwork;

        // Hardware - Disk
        QString diskName;
        QString diskPath;
        qlonglong diskSize;
        QString diskFormat;
        bool createNewDisk;

        // Accelerator
        QHash<QString, QString> accelerator;

};

#endif // MACHINE_H

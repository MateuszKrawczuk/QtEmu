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
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

// Local
#include "qemu.h"
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

        QStringList bootOrder() const;
        void setBootOrder(const QStringList &bootOrder);

        // Methods
        void addBootOrder(const QString bootOrder);
        void removeBootOrder(const QString bootOrder);
        void removeAllBootOrder();

    protected:

    private:
        bool m_bootMenu;
        bool m_kernelBootEnabled;
        QString m_kernelPath;
        QString m_initrdPath;
        QString m_kernelArgs;
        QStringList m_bootOrder;

};

class Media {

    public:
        explicit Media();
        ~Media();

        QString name() const;
        void setName(const QString &name);

        QString path() const;
        void setPath(const QString &path);

        qlonglong size() const;
        void setSize(const qlonglong &size);

        QString type() const;
        void setType(const QString &type);

        QString format() const;
        void setFormat(const QString &format);

        QString driveInterface() const;
        void setDriveInterface(const QString &driveInterface);

        QString cache() const;
        void setCache(const QString &cache);

        QString IO() const;
        void setIO(const QString &IO);

        QUuid uuid() const;
        void setUuid(const QUuid &uuid);

    protected:

    private:
        QString m_name;
        QString m_path;
        qlonglong m_size;
        QString m_type;
        QString m_format;
        QString m_driveInterface;
        QString m_cache;
        QString m_IO;
        QUuid m_uuid;
};

class Machine: public QObject {
    Q_OBJECT

    public:
        explicit Machine(QObject *parent = nullptr);
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

        int getCPUCount() const;
        void setCPUCount(const int &value);

        int getSocketCount() const;
        void setSocketCount(const int &value);

        int getCoresSocket() const;
        void setCoresSocket(const int &value);

        int getThreadsCore() const;
        void setThreadsCore(const int &value);

        int getMaxHotCPU() const;
        void setMaxHotCPU(const int &value);

        QString getGPUType() const;
        void setGPUType(const QString &value);

        QString getKeyboard() const;
        void setKeyboard(const QString &value);

        qlonglong getRAM() const;
        void setRAM(const qlonglong &value);

        QStringList getAudio() const;
        void setAudio(const QStringList &value);

        QString getHostSoundSystem() const;
        void setHostSoundSystem(const QString &value);

        bool getUseNetwork() const;
        void setUseNetwork(bool value);

        QList<Media> getMedia() const;
        void addMedia(const Media value);

        QStringList getAccelerator() const;
        void setAccelerator(const QStringList &value);

        Boot getBoot() const;
        void setBoot(const Boot &boot);

        // Methods
        void addAudio(const QString audio);
        void removeAudio(const QString audio);
        void removeAllAudioCards();

        void addAccelerator(const QString accel);
        void removeAccelerator(const QString accel);
        void removeAllAccelerators();

        QString getAudioLabel();
        QString getAcceleratorLabel();

        void runMachine(QEMU *QEMUGlobalObject);
        void stopMachine();
        void resetMachine();
        void pauseMachine();

        void saveMachine();
        void insertMachineConfigFile();

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
        int CPUCount;
        int socketCount;
        int coresSocket;
        int threadsCore;
        int maxHotCPU;

        // Hardware - GPU
        QString GPUType;
        QString keyboard;

        // Hardware - RAM
        qlonglong RAM;

        // Hardware - Audio
        QStringList audio;
        QString hostSoundSystem;

        // Hardware - Network
        bool useNetwork;

        // Hardware - media
        QList<Media> media;

        // Accelerator
        QStringList accelerator;

        // Boot
        Boot m_boot;

        // Process
        QProcess *m_machineProcess;

        // Messages
        /*QMessageBox *m_saveMachineMessageBox;
        QMessageBox *m_machineConfigMessageBox;
        QMessageBox *m_machineStandardOutMessageBox;
        QMessageBox *m_machineErrorOutMessageBox;*/

        // Methods
        QProcessEnvironment buildEnvironment();
        QStringList generateMachineCommand();
};

#endif // MACHINE_H

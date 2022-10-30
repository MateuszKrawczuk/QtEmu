/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include <QTcpSocket>
#include <QHash>
#include <QUuid>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

// Local
#include "qemu.h"
#include "boot.h"
#include "media.h"
#include "machineutils.h"
#include "utils/logger.h"

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

        QString getType() const;
        void setType(const QString &value);

        QString getPath() const;
        void setPath(const QString &value);

        QString getConfigPath() const;
        void setConfigPath(const QString &value);

        QUuid getUuid() const;
        void setUuid(const QUuid &value);

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

        QList<Media *> getMedia() const;
        void addMedia(Media *media);

        QStringList getAccelerator() const;
        void setAccelerator(const QStringList &value);

        Boot *getBoot() const;
        void setBoot(Boot *value);

        // Methods
        void addAudio(const QString audio);
        void removeAudio(const QString audio);
        void removeAllAudioCards();

        void addAccelerator(const QString accel);
        void removeAccelerator(const QString accel);
        void removeAllAccelerators();

        void removeAllMedia();

        QString getAudioLabel();
        QString getAcceleratorLabel();

        void runMachine(QEMU *QEMUGlobalObject);
        void stopMachine();
        void resetMachine();
        void pauseMachine();
        bool saveMachine();
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
        QString type;
        QString path;
        QString configPath;
        QUuid uuid;
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
        QList<Media *> media;

        // Accelerator
        QStringList accelerator;

        // Boot
        Boot *boot;

        // Process
        QProcess *m_machineProcess;
        QTcpSocket *m_machineTcpSocket;

        // Messages
        QMessageBox *m_saveMachineMessageBox;
        QMessageBox *m_machineConfigMessageBox;
        QMessageBox *m_machineStandardOutMessageBox;
        QMessageBox *m_machineErrorOutMessageBox;
        QMessageBox *m_machineBinaryErrorMessageBox;
        QMessageBox *m_failConnectErrorMessageBox;

        // Methods
        QProcessEnvironment buildEnvironment();
        QStringList generateMachineCommand();
        void failConnectMachine();
};
#endif // MACHINE_H

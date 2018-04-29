/****************************************************************************
**
** Copyright (C) 2006-2007 Urs Wolfer <uwolfer @ fwo.ch>
**
** This file is part of QtEmu.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU Library General Public License
** along with this library; see the file COPYING.LIB.  If not, write to
** the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA 02110-1301, USA.
**
****************************************************************************/

#include "machineprocess.h"

#include "config.h"

#include <QCoreApplication>
#include <QSettings>
#include <QTime>
#include <QDir>

MachineProcess::MachineProcess(QObject *parent)
               :QProcess(parent),
                bootFromCdEnabled(false),
                bootFromFloppyEnabled(false),
                snapshotEnabled(false),
                networkEnabled(false),
                mouseEnabled(false),
                timeEnabled(false),
                additionalOptionsEnabled(false),
                memoryInt(-1),
                cpuInt(-1)
{
}

void MachineProcess::start()
{
    QStringList arguments;

    // Append the additional parameters first so wrapper programs like vde
    // can be used to start the virtual machine, e.g. vdeq kvm or vdeq qemu
    // where vdeq is specified as the command and kvm/qemu as the additional
    // option or parameter.
    if (additionalOptionsEnabled && !additionalOptionsString.isEmpty())
      arguments << additionalOptionsString.split(" ", QString::SkipEmptyParts);

    if (snapshotEnabled)
        arguments << "-snapshot";

    if (networkEnabled)
    {
        if (!networkCustomOptionsString.isEmpty())
          arguments << networkCustomOptionsString.split(" ", QString::SkipEmptyParts);
        else
          arguments << "-net" << "nic" << "-net" << "user";
    }
    else
        arguments << "-net" << "none";

    if (soundEnabled)
        arguments << "-soundhw" << "es1370";

    if (!cdRomPathString.isEmpty())
    {
        arguments << "-cdrom" << cdRomPathString;
        if (bootFromCdEnabled)
            arguments << "-boot" << "d";
    }

    if (!floppyDiskPathString.isEmpty())
    {
        arguments << "-fda" << floppyDiskPathString;
        if (bootFromFloppyEnabled)
            arguments << "-boot" << "a";
    }

    if (memoryInt > 0)
      arguments << "-m" << QString::number(memoryInt);

    if (cpuInt > 1)
      arguments << "-smp" << QString::number(cpuInt);

    if (mouseEnabled)
        arguments << "-usb" << "-usbdevice" << "tablet";

    if (timeEnabled)
        arguments << "-localtime";

    // Add the VM image name...
    arguments << pathString;

#ifdef DEVELOPER
    QString debugString = QString();
    for (int i = 0; i < arguments.size(); ++i)
        debugString = debugString + arguments.at(i).toLocal8Bit().constData() + ' ';
    qDebug(debugString.toLocal8Bit().constData());
#endif

    connect(this, SIGNAL(finished(int)), SLOT(afterExitExecute()));

    QSettings settings("QtEmu", "QtEmu");

    QString command = settings.value("beforeStart").toString();
    if (!command.isEmpty())
    {
        QStringList commandList;
        commandList = command.split("\n");
        for (int i = 0; i < commandList.size(); ++i)
        {
            QProcess::start(commandList.at(i).toLocal8Bit().constData());
            while (waitForFinished())
            {
                QTime sleepTime = QTime::currentTime().addMSecs(5);
                while (QTime::currentTime() < sleepTime)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
    }

#ifndef Q_OS_WIN32
    QProcess::start(settings.value("command", "qemu").toString(), arguments);
#elif defined(Q_OS_WIN32)
    arguments << "-L" << ".";
    QString qemuCommand = settings.value("command", QCoreApplication::applicationDirPath() + "/qemu/qemu.exe").toString();
    QDir *path = new QDir(qemuCommand);
    path->cdUp();
    setWorkingDirectory(path->path());
    QProcess::start(qemuCommand, arguments);
#endif
}

void MachineProcess::afterExitExecute()
{
    QSettings settings("QtEmu", "QtEmu");

    QString command = settings.value("afterExit").toString();
    if (!command.isEmpty())
    {
        QStringList commandList;
        commandList = command.split("\n");
        for (int i = 0; i < commandList.size(); ++i)
        {
            QProcess::start(commandList.at(i).toLocal8Bit().constData());
            while (waitForFinished())
            {
                QTime sleepTime = QTime::currentTime().addMSecs(5);
                while (QTime::currentTime() < sleepTime)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
    }
}

void MachineProcess::path(const QString &newPath)
{
    pathString = newPath;
}

void MachineProcess::cdRomPath(const QString &newPath)
{
    cdRomPathString = newPath;
}

void MachineProcess::cdBoot(int value)
{
    bootFromCdEnabled = (value == Qt::Checked);
}

void MachineProcess::floppyDiskPath(const QString &newPath)
{
    floppyDiskPathString = newPath;
}

void MachineProcess::floppyBoot(int value)
{
    bootFromFloppyEnabled = (value == Qt::Checked);
}

void MachineProcess::snapshot(int value)
{
#ifdef DEVELOPER
    snapshotEnabled = true;
#else
    snapshotEnabled = (value == Qt::Checked);
#endif
}

void MachineProcess::network(int value)
{
    networkEnabled = (value == Qt::Checked);
}

void MachineProcess::sound(int value)
{
    soundEnabled = (value == Qt::Checked);
}

void MachineProcess::time(int value)
{
    timeEnabled = (value == Qt::Checked);
}

void MachineProcess::mouse(int value)
{
    mouseEnabled = (value == Qt::Checked);
}

void MachineProcess::useAdditionalOptions(int value)
{
    additionalOptionsEnabled = (value == Qt::Checked);
}

void MachineProcess::memory(int value)
{
    memoryInt = value;
}

void MachineProcess::cpu(int value)
{
    cpuInt = value;
}

void MachineProcess::networkCustomOptions(const QString& options)
{
    networkCustomOptionsString = options;
}

void MachineProcess::additionalOptions(const QString& options)
{
    additionalOptionsString = options;
}

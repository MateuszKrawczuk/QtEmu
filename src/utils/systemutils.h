// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

// Qt
#include <QLabel>
#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>

#include <QDebug>

// Local
#include "../qemu.h"

// GNU
#ifdef Q_OS_LINUX
#include <sys/sysinfo.h>
#endif

// Windows
#ifdef Q_OS_WIN
#include <windows.h>
#endif

// FreeBSD
#ifdef Q_OS_FREEBSD
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#endif

// MacOS
#ifdef Q_OS_MAC
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

class SystemUtils {

    public:
        SystemUtils();
        ~SystemUtils();

        static void showMessage(QString title, QString text, QMessageBox::Icon severityLevel);

        static void getTotalMemory(int &totalRAM);

        static void setCPUTypesx86(QComboBox *CPUType);
        static void setGPUTypes(QComboBox *GPUType);
        static void setKeyboardLayout(QComboBox *keyboardLayout);
        static QHash<QString, QString> getSoundCards();
        static QHash<QString, QString> getAccelerators();
        static QMap<QString, QString> getMediaDevices();

        static QString getOsIcon(const QString &osVersion);

        static bool createDisk(QEMU *qemuGlobalObject,
                               const QString &diskName,
                               const QString &format,
                               const double size,
                               bool useEncryption);

    private:

};

#endif // SYSTEMUTILS_H

/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

// Local
#include "qemu.h"

#include <QCoreApplication>

/**
 * @brief QEMU object
 * @param parent, parent widget
 *
 * QEMU object with properties of the emulator
 */
QEMU::QEMU(QObject *parent) : QObject(parent)
{
    QSettings settings;
    settings.beginGroup("Configuration");

    QString qemuBinariesPath;
    QString qemuImgPath;
    QString qemuBiosPath;
    #ifdef Q_OS_LINUX
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/bin")).toString();
    qemuImgPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/bin")).toString();
    #endif
    #ifdef Q_OS_WIN
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("C:\\Program Files\\qemu")).toString();
    qemuImgPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("C:\\Program Files\\qemu")).toString();
    #endif
    #ifdef Q_OS_MACOS
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/local/bin/")).toString();
    qemuImgPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/local/bin/")).toString();
    #endif
    #ifdef Q_OS_FREEBSD
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/local/bin/")).toString();
    qemuImgPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/local/bin/")).toString();
    #endif
    settings.endGroup();
    settings.sync();

    settings.beginGroup("Configuration");
    #ifdef Q_OS_LINUX
    qemuBiosPath = settings.value("qemuBiosPath", QDir::toNativeSeparators("/usr/share/qemu")).toString();
    #endif
    #ifdef Q_OS_WIN
    qemuBiosPath = settings.value("qemuBiosPath", QDir::toNativeSeparators(qemuBinariesPath + "\\share\\qemu")).toString();
    #endif
    #ifdef Q_OS_MACOS
    qemuBiosPath = settings.value("qemuBiosPath", QDir::toNativeSeparators("/usr/local/share/qemu")).toString();
    #endif
    #ifdef Q_OS_FREEBSD
    qemuBiosPath = settings.value("qemuBiosPath", QDir::toNativeSeparators("/usr/local/share/qemu")).toString();
    #endif
    settings.endGroup();

    this->setQEMUImgPath(qemuImgPath);
    this->setQEMUBinaries(qemuBinariesPath);
    this->m_BIOSDirectory = qemuBiosPath;

    qDebug() << "QEMU object created";
}

QEMU::~QEMU()
{
    qDebug() << "QEMU object destroyed";
}

/**
 * @brief Get the path of the qemu-img binary
 * @return path of qemu-img
 *
 * Get the path of the qemu-img binary
 */
QString QEMU::QEMUImgPath() const
{
    return m_QEMUImgPath;
}

/**
 * @brief Set the path of the qemu-img binary
 * @param path, path of qemu-img
 *
 * Set the path of the qemu-img binary
 */
void QEMU::setQEMUImgPath(const QString &path)
{    
#ifdef Q_OS_LINUX
    QString qemuImgPath = QDir::toNativeSeparators(path + "/qemu-img");
#endif
#ifdef Q_OS_WIN
    QString qemuImgPath = QDir::toNativeSeparators(path + "/qemu-img.exe");
#endif
#ifdef Q_OS_MACOS
    QString qemuImgPath = QDir::toNativeSeparators(path + "/qemu-img");
#endif
#ifdef Q_OS_FREEBSD
    QString qemuImgPath = QDir::toNativeSeparators(path + "/qemu-img");
#endif

    this->m_QEMUImgPath = qemuImgPath;
}

/**
 * @brief Get all QEMU binaries
 * @return QMap with all the qemu binaries location
 *
 * Get all QEMU binaries
 */
QMap<QString, QString> QEMU::QEMUBinaries() const
{
    return m_QEMUBinaries;
}

/**
 * @brief Get the path where one binary are located
 * @param binary, binary to be located
 * @return path of the binary
 *
 * Get the path where one binary are located
 */
QString QEMU::getQEMUBinary(const QString &binary) const
{
    #ifdef Q_OS_WIN
    // Dodaj .exe jeśli nie ma
    QString searchBinary = binary;
    if (!searchBinary.endsWith(".exe")) {
        searchBinary += ".exe";
    }
    
    // Szukaj wersji z 'w'
    QString windowsBinary = searchBinary;
    windowsBinary.replace(".exe", "w.exe");
    
    if (this->m_QEMUBinaries.contains(windowsBinary)) {
        return this->m_QEMUBinaries.value(windowsBinary);
    }
    
    // Jeśli nie znaleziono wersji z 'w', spróbuj bez 'w'
    if (this->m_QEMUBinaries.contains(searchBinary)) {
        return this->m_QEMUBinaries.value(searchBinary);
    }
    #else
    if (this->m_QEMUBinaries.contains(binary)) {
        return this->m_QEMUBinaries.value(binary);
    }
    #endif
    
    return QString();
}

/**
 * @brief Set the QEMU binaries
 * @param path, path where the QEMU binaries are located
 *
 * Set the QEMU binaries
 */
void QEMU::setQEMUBinaries(const QString &path)
{
    this->m_QEMUBinaries.clear();

    qDebug() << "Searching for QEMU binaries in:" << path;

    // Add common QEMU installation paths for Windows
    QStringList searchPaths;
    searchPaths << path;
    #ifdef Q_OS_WIN
    QString scoopPath = QDir::homePath() + "\\scoop\\apps\\qemu\\current";
    searchPaths << "C:\\Program Files\\qemu"
                << "C:\\Program Files (x86)\\qemu"
                << "C:\\Program Files\\QEMU"
                << "C:\\Program Files (x86)\\QEMU"
                << QCoreApplication::applicationDirPath() + "\\qemu"
                << QCoreApplication::applicationDirPath() + "\\QEMU"
                << scoopPath;
    #endif

    bool foundAny = false;
    for (const QString &searchPath : searchPaths) {
        QDir dir(searchPath);
        if (!dir.exists()) {
            continue;
        }
        QStringList nameFilters;
        #ifdef Q_OS_WIN
        nameFilters << "qemu-system-*.exe";
        #else
        nameFilters << "qemu-system-*";
        #endif

        // Najpierw sprawdź pliki bezpośrednio w katalogu
        QStringList directFiles = dir.entryList(nameFilters, QDir::Files | QDir::Executable);
        for (const QString &file : directFiles) {
            QString filePath = dir.filePath(file);
            #ifdef Q_OS_WIN
            if (file.contains("qemu-system-")) {
                this->m_QEMUBinaries.insert(file, filePath);
                foundAny = true;
            }
            #endif
        }

        // Następnie sprawdź podkatalogi
        QDirIterator it(searchPath, nameFilters, QDir::Files | QDir::Executable, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QString fileName = it.fileName();
            
            #ifdef Q_OS_WIN
            if (fileName.contains("qemu-system-")) {
                this->m_QEMUBinaries.insert(fileName, filePath);
                foundAny = true;
            }
            #else
            if (!fileName.contains("-w64") && !fileName.contains("-w32")) {
                this->m_QEMUBinaries.insert(fileName, filePath);
                foundAny = true;
            }
            #endif
        }
    }

    if (!foundAny) {
        qDebug() << "Warning: No QEMU binaries found in any search paths";
    } else {
        qDebug() << "Found" << m_QEMUBinaries.size() << "QEMU binaries";
    }
}

/**
 * @brief Get the BIOS directory
 * @return path to the BIOS directory
 *
 * Get the path to the directory containing BIOS files
 */
QString QEMU::BIOSDirectory() const
{
    return m_BIOSDirectory;
}

/**
 * @brief Get available BIOS files
 * @return list of available BIOS files
 *
 * Get a list of available BIOS files in the BIOS directory
 */
QStringList QEMU::availableBIOSFiles() const
{
    QStringList biosFiles;
    QDir biosDir(m_BIOSDirectory);
    
    if (biosDir.exists()) {
        QStringList filters;
        filters << "*.bin" << "*.rom" << "*.fd";
        biosFiles = biosDir.entryList(filters, QDir::Files | QDir::Readable, QDir::Name);
    }
    
    return biosFiles;
}

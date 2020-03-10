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
    #ifdef Q_OS_LINUX
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/bin")).toString();
    qemuImgPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/bin")).toString();
    #endif
    #ifdef Q_OS_WIN
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("C:\\")).toString();
    qemuImgPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("C:\\")).toString();
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

    this->setQEMUImgPath(qemuImgPath);
    this->setQEMUBinaries(qemuBinariesPath);

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
void QEMU::setQEMUImgPath(const QString path)
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
QString QEMU::getQEMUBinary(const QString binary) const
{
    return this->m_QEMUBinaries.value(binary);
}

/**
 * @brief Set the QEMU binaries
 * @param path, path where the QEMU binaries are located
 *
 * Set the QEMU binaries
 */
void QEMU::setQEMUBinaries(const QString path)
{
    this->m_QEMUBinaries.clear();

    QDirIterator it(path, QStringList() << "qemu-system-*", QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
#ifdef Q_OS_WIN
        if (it.fileName().contains("w")) {
#else
        if (!it.fileName().contains("w")) {
#endif
            this->m_QEMUBinaries.insert(it.fileName(), it.filePath());
        }
    }
}

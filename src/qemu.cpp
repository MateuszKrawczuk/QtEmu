/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

// Local
#include "qemu.h"

QEMU::QEMU(QObject *parent) : QObject(parent) {

    QSettings settings;
    settings.beginGroup("Configuration");

    QString qemuBinariesPath;
    QString qemuImgPath;
    #ifdef Q_OS_LINUX
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("/usr/bin")).toString();
    qemuImgPath = settings.value("qemuImgBinaryPath", QDir::toNativeSeparators("/usr/bin")).toString();
    #endif
    #ifdef Q_OS_WIN
    qemuBinariesPath = settings.value("qemuBinaryPath", QDir::toNativeSeparators("C:\\")).toString();
    qemuImgPath = settings.value("qemuImgBinaryPath", QDir::toNativeSeparators("C:\\")).toString();
    #endif
    #ifdef Q_OS_MACOS
    qemuBinariesPath = QDir::toNativeSeparators("");
    qemuImgPath = QDir::toNativeSeparators("");
    #endif
    #ifdef Q_OS_FREEBSD
    qemuBinariesPath = QDir::toNativeSeparators("");
    qemuImgPath = QDir::toNativeSeparators("");
    #endif

    settings.endGroup();
    settings.sync();

    this -> setQEMUImgPath(qemuImgPath);
    this -> setQEMUBinaries(qemuBinariesPath);

    qDebug() << "QEMU object created";
}

QEMU::~QEMU() {
    qDebug() << "QEMU object destroyed";
}

QString QEMU::QEMUImgPath() const {
    return m_QEMUImgPath;
}

void QEMU::setQEMUImgPath(const QString path) {

    QString qemuImgPath;
    #ifdef Q_OS_LINUX
    qemuImgPath = path + QDir::toNativeSeparators("/") + "qemu-img";
    #endif
    #ifdef Q_OS_WIN
    qemuImgPath = path;
    #endif
    #ifdef Q_OS_MACOS
    qemuImgPath = "";
    #endif
    #ifdef Q_OS_FREEBSD
    qemuImgPath = "";
    #endif

    QFile qemuImgFile(qemuImgPath);
    if (qemuImgFile.exists()) {
        this->m_QEMUImgPath = qemuImgPath;
    } else {
        this->m_QEMUImgPath = "";
        qDebug() << "Cannot find qemu-img binary";
    }
}

QMap<QString, QString> QEMU::QEMUBinaries() const {
    return m_QEMUBinaries;
}

QString QEMU::getQEMUBinary(const QString binary) const {
    return this -> m_QEMUBinaries.value(binary);
}

void QEMU::setQEMUBinaries(const QString path) {
    this->m_QEMUBinaries.clear();

    QDirIterator it(path, QStringList() << "qemu-system-*", QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        this->m_QEMUBinaries.insert(it.fileName(), it.filePath());
    }
}

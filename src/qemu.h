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

#ifndef QEMU_H
#define QEMU_H

// Qt
#include <QObject>
#include <QDirIterator>
#include <QFile>
#include <QMap>
#include <QSettings>
#include <QJsonDocument>

#include <QDebug>

class QEMU : public QObject {
    Q_OBJECT

    public:
        explicit QEMU(QObject *parent = nullptr);
        ~QEMU();

        QString QEMUImgPath() const;
        void setQEMUImgPath(const QString &path);

        QMap<QString, QString> QEMUBinaries() const;
        QString getQEMUBinary(const QString &binary) const;
        void setQEMUBinaries(const QString &path);
        
        QString BIOSDirectory() const;
        QStringList availableBIOSFiles() const;
        
        QString getQEMUVersion() const;
        bool isQEMUVersionGreaterThan(int major, int minor = 0) const;

    protected:

    private:
        QString m_QEMUImgPath;
        QString m_BIOSDirectory;
        QMap<QString, QString> m_QEMUBinaries;

};

#endif // QEMU_H

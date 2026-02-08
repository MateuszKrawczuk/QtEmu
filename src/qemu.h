// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

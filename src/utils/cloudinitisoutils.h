// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CLOUDINITISOUTILS_H
#define CLOUDINITISOUTILS_H

// Qt
#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QUuid>
#include <QTemporaryDir>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDebug>

// Local
#include "../machine.h"

class CloudInitIsoUtils : public QObject {
    Q_OBJECT

    public:
        explicit CloudInitIsoUtils(QObject *parent = nullptr);
        ~CloudInitIsoUtils();

        static QString generateCloudInitISO(Machine *machine);
        static bool cleanupCloudInitISO(const QString &isoPath);

    private:
        static QString generateUserData(Machine *machine);
        static QString generateMetaData(Machine *machine);
        static QString hashPassword(const QString &password);
        static QString findISOCreationTool();
        static bool createISO(const QString &sourceDir, const QString &isoPath);
};

#endif // CLOUDINITISOUTILS_H

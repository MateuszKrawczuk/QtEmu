// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINEUTILS_H
#define MACHINEUTILS_H

// Qt
#include <QObject>
#include <QSettings>
#include <QUuid>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutableHashIterator>
#include <QMessageBox>
#include <QDebug>

// Local
#include "utils/systemutils.h"

class Machine; // Forward declaration :'(

class MachineUtils : public QObject {
    Q_OBJECT

    public:
        explicit MachineUtils(QObject *parent = nullptr);
        ~MachineUtils();

        static QJsonObject readMachineFile(QString machinePath);
        static void fillMachineObject(Machine *machine,
                                      QJsonObject machineJSON, QString machineConfigPath);
        static bool deleteMachine(const QUuid machineUuid);

        static QStringList getSoundCards(QJsonArray soundCardsArray);
        static QStringList getAccelerators(QJsonArray acceleratorsArray);
        static QStringList getMediaDevices(QJsonArray mediaDevicesArray);

    public slots:

    protected:

    private:

};

#endif // MACHINEUTILS_H

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

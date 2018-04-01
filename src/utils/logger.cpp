/*
 * This file is part of QtEmu project.
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

// Qt

// Local
#include "logger.h"

Logger::Logger() {

    QSettings settings;
    settings.beginGroup("DataFolder");

    QString logDirectoryPath = settings.value("QtEmuLogs").toString();
    settings.endGroup();

    logsFile = new QFile;
    logsFile -> setFileName(logDirectoryPath.append("/qtemu.log"));
    logsFile -> open(QIODevice::Append); // TODO: Check if open the file fails and if write is allowed

    qDebug() << "Logger created";
}

Logger::~Logger() {

    this -> logsFile -> close();

    qDebug() << "Logger destroyed";
}

void Logger::logMachineCreation(const QString &message) {

    QString logMessage = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ").append(message);

    QTextStream fileStream(this -> logsFile);
    fileStream.setCodec("UTF-8");

    if (this -> logsFile -> isWritable()) {
        fileStream << logMessage;
    }

}

void Logger::logMachineAction(const QString &message) {

    QString logMessage = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ").append(message);

    QTextStream fileStream(this -> logsFile);
    fileStream.setCodec("UTF-8");

    if (this -> logsFile -> isWritable()) {
        fileStream << logMessage;
    }

}

void Logger::logQtemuAction(const QString &message) {


}

void Logger::logQtemuError(const QString &message) {


}

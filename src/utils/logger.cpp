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

// Local
#include "logger.h"

Logger::Logger(QObject *parent) : QObject(parent)
{
    qDebug() << "Logger created";
}

Logger::~Logger()
{
    qDebug() << "Logger destroyed";
}

void Logger::logMachineCreation(const QString &fileLocation,
                                const QString &machineName, const QString &message) {

    QString path = fileLocation + "/logs/" + machineName.toLower().replace(" ", "_") + ".log";

    QFile machineLogsFile(path);
    if (!machineLogsFile.open(QIODevice::Append)) {
        QMessageBox *m_logMachineMessageBox = new QMessageBox();
        m_logMachineMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_logMachineMessageBox->setIcon(QMessageBox::Critical);
        m_logMachineMessageBox->setText(tr("<p>Problem with the log</p>"));
        m_logMachineMessageBox->exec();
        return;
    }

    QTextStream fileStream(&machineLogsFile);
    fileStream.setCodec("UTF-8");

    if (machineLogsFile.isWritable()) {
        fileStream << message;
    }

    if (machineLogsFile.isOpen()) {
        machineLogsFile.close();
    }

}

void Logger::logQtemuAction(const QString &message) {

    QSettings settings;
    settings.beginGroup("DataFolder");
    QString logDirectoryPath = settings.value("QtEmuLogs").toString();
    settings.endGroup();

    QString logMessage = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ").append(message).append("\n");

    QFile logsFile(logDirectoryPath.append("/qtemu.log"));
    if (!logsFile.open(QIODevice::Append)) {
        QMessageBox *m_logQtEmuMessageBox = new QMessageBox();
        m_logQtEmuMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_logQtEmuMessageBox->setIcon(QMessageBox::Critical);
        m_logQtEmuMessageBox->setText(tr("<p>Problem with the log</p>"));
        m_logQtEmuMessageBox->exec();
        return;
    }

    QTextStream fileStream(&logsFile);
    fileStream.setCodec("UTF-8");

    if (logsFile.isWritable()) {
        fileStream << logMessage;
    }

    if (logsFile.isOpen()) {
        logsFile.close();
    }

}

void Logger::logQtemuError(const QString &message) {

    QSettings settings;
    settings.beginGroup("DataFolder");

    QString logDirectoryPath = settings.value("QtEmuLogs").toString();
    settings.endGroup();

    QString logMessage = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ").append(message).append("\n");;

    QFile logsFile(logDirectoryPath.append("/qtemu.err"));
    if (!logsFile.open(QIODevice::Append)) {
        QMessageBox *m_logQtEmuErrorMessageBox = new QMessageBox();
        m_logQtEmuErrorMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_logQtEmuErrorMessageBox->setIcon(QMessageBox::Critical);
        m_logQtEmuErrorMessageBox->setText(tr("<p>Problem with the log</p>"));
        m_logQtEmuErrorMessageBox->exec();
        return;
    }

    QTextStream fileStream(&logsFile);
    fileStream.setCodec("UTF-8");

    if (logsFile.isWritable()) {
        fileStream << logMessage;
    }

    if (logsFile.isOpen()) {
        logsFile.close();
    }

}

// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

/**
 * @brief Log the machine creation actions
 * @param fileLocation, log file location
 * @param machineName, name of the machine
 * @param message, message to be logged
 *
 * Log the machine creation actions
 */
void Logger::logMachineCreation(const QString &fileLocation,
                                const QString &machineName,
                                const QString &message)
{
    QString path = fileLocation + "/logs/" + machineName.toLower().replace(" ", "_") + ".log";

    QFile machineLogsFile(path);
    if (!machineLogsFile.open(QIODevice::Append)) {
        QMessageBox *m_logMachineMessageBox = new QMessageBox();
        m_logMachineMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_logMachineMessageBox->setIcon(QMessageBox::Critical);
        m_logMachineMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_logMachineMessageBox->setText(tr("<p>Problem writing in the log</p>"));
        m_logMachineMessageBox->exec();
        return;
    }

    QTextStream fileStream(&machineLogsFile);
    fileStream.setEncoding(QStringConverter::Utf8);

    if (machineLogsFile.isWritable()) {
        fileStream << message;
    }

    if (machineLogsFile.isOpen()) {
        machineLogsFile.close();
    }
}

/**
 * @brief Log qtemu actions
 * @param message, message to be logged
 *
 * Log the qtemu actions
 */
void Logger::logQtemuAction(const QString &message)
{
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
        m_logQtEmuMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_logQtEmuMessageBox->setText(tr("<p>Problem writing in the log</p>"));
        m_logQtEmuMessageBox->exec();
        return;
    }

    QTextStream fileStream(&logsFile);
    fileStream.setEncoding(QStringConverter::Utf8);

    if (logsFile.isWritable()) {
        fileStream << logMessage;
    }

    if (logsFile.isOpen()) {
        logsFile.close();
    }
}

/**
 * @brief Log the qtemu errors
 * @param message, message to be logged
 *
 * Log the qtemu errors
 */
void Logger::logQtemuError(const QString &message)
{
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
        m_logQtEmuErrorMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_logQtEmuErrorMessageBox->setText(tr("<p>Problem writing in the log</p>"));
        m_logQtEmuErrorMessageBox->exec();
        return;
    }

    QTextStream fileStream(&logsFile);
    fileStream.setEncoding(QStringConverter::Utf8);

    if (logsFile.isWritable()) {
        fileStream << logMessage;
    }

    if (logsFile.isOpen()) {
        logsFile.close();
    }
}

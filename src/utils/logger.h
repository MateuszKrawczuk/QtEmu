// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef LOGGER_H
#define LOGGER_H

// Qt
#include <QObject>
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

class Logger : public QObject {
    Q_OBJECT

    public:
        explicit Logger(QObject *parent = nullptr);
        ~Logger();

        static void logMachineCreation(const QString &fileLocation,
                                       const QString &machineName,
                                       const QString &message);
        static void logQtemuAction(const QString &message);
        static void logQtemuError(const QString &message);

    public slots:

    protected:

    private:

};

#endif // LOGGER_H

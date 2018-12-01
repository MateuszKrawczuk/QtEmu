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
#include "qtemualerts.h"

QtEmuAlerts::QtEmuAlerts(QObject *parent) : QObject(parent)
{
    qDebug() << "QtEmuAlerts created";
}

QtEmuAlerts::~QtEmuAlerts()
{
    qDebug() << "QtEmuAlerts destroyed";
}

void QtEmuAlerts::qtemuWarning(const QString &message)
{
    QMessageBox *m_logMachineMessageBox = new QMessageBox();
    m_logMachineMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
    m_logMachineMessageBox->setIcon(QMessageBox::Critical);
    m_logMachineMessageBox->setText(tr("<p>Problem with the log</p>"));
    m_logMachineMessageBox->exec();
}

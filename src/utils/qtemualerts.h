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

#ifndef QTEMUALERTS_H
#define QTEMUALERTS_H

// Qt
#include <QObject>
#include <QMessageBox>
#include <QDebug>

class QtEmuAlerts : public QObject {
    Q_OBJECT

    public:
        explicit QtEmuAlerts(QObject *parent = nullptr);
        ~QtEmuAlerts();

        static void qtemuWarning(const QString &message);

    public slots:

    protected:

    private:

};
#endif // QTEMUALERTS_H

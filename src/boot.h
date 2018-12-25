/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

#ifndef BOOT_H
#define BOOT_H

// Qt
#include <QObject>
#include <QDebug>

class Boot: public QObject {
    Q_OBJECT

    public:
        explicit Boot(QObject *parent = nullptr);
        ~Boot();

        bool bootMenu() const;
        void setBootMenu(bool bootMenu);

        bool kernelBootEnabled() const;
        void setKernelBootEnabled(bool kernelBootEnabled);

        QString kernelPath() const;
        void setKernelPath(const QString &kernelPath);

        QString initrdPath() const;
        void setInitrdPath(const QString &initrdPath);

        QString kernelArgs() const;
        void setKernelArgs(const QString &kernelArgs);

        QStringList bootOrder() const;
        void setBootOrder(const QStringList &bootOrder);

        // Methods
        void addBootOrder(const QString bootOrder);
        void removeBootOrder(const QString bootOrder);
        void removeAllBootOrder();

    protected:

    private:
        bool m_bootMenu;
        bool m_kernelBootEnabled;
        QString m_kernelPath;
        QString m_initrdPath;
        QString m_kernelArgs;
        QStringList m_bootOrder;

};

#endif // BOOT_H

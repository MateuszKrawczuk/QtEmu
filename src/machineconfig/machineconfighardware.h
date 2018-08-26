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
#ifndef MACHINECONFIGHARDWARE_H
#define MACHINECONFIGHARDWARE_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>

// Local
#include "../machine.h"
#include "../newmachine/hardwarepage.h"

class MachineConfigHardware : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigHardware(Machine *machine,
                                       QWidget *parent = nullptr);
        ~MachineConfigHardware();
        QWidget *m_hardwarePageWidget;

    signals:

    public slots:

    private slots:

    protected:

    private:
        QTabWidget *m_hardwareTabWidget;
        QVBoxLayout *m_hardwarePageLayout;

};
#endif // MACHINECONFIGHARDWARE_H

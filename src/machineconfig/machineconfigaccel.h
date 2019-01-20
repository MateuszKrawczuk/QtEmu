/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2019 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#ifndef MACHINECONFIGACCEL_H
#define MACHINECONFIGACCEL_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTreeWidget>

// Local
#include "../machine.h"
#include "../utils/systemutils.h"

class MachineConfigAccel : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigAccel(Machine *machine,
                                    QWidget *parent = nullptr);
        ~MachineConfigAccel();
        QWidget *m_acceleratorPageWidget;

        // Methods
        void saveAccelData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_accelTreeLayout;
        QVBoxLayout *m_acceleratorLayout;

        QToolButton *m_moveUpAccelToolButton;
        QToolButton *m_moveDownAccelToolButton;

        QTreeWidget *m_acceleratorTree;

        QTreeWidgetItem *m_treeItem;

        Machine *m_machine;

        // Methods
        void moveUpButton();
        void moveDownButton();
};

#endif // MACHINECONFIGACCEL_H

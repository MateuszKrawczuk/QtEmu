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
#ifndef MACHINECONFIGBOOT_H
#define MACHINECONFIGBOOT_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTreeWidget>
#include <QCheckBox>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

// Local
#include "../machine.h"

class MachineConfigBoot : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigBoot(Machine *machine,
                                   QWidget *parent = 0);
        ~MachineConfigBoot();
        QWidget *m_bootPageWidget;

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_bootTreeLayout;
        QVBoxLayout *m_bootPageLayout;
        QGridLayout *m_kernelLayout;

        QTreeWidget *m_bootTree;
        QTreeWidgetItem *m_floppyTreeItem;
        QTreeWidgetItem *m_CDROMTreeItem;
        QTreeWidgetItem *m_hardDiskTreeItem;
        QTreeWidgetItem *m_networkTreeItem;

        QCheckBox *m_bootMenuCheckBox;
        QCheckBox *m_kernelBootCheckBox;

        QToolButton *m_moveUpToolButton;
        QToolButton *m_moveDownToolButton;

        QLabel *m_kernelPathLabel;
        QLabel *m_initrdLabel;
        QLabel *m_kernelArgsLabel;

        QLineEdit *m_kernelPathLineEdit;
        QLineEdit *m_initredLineEdit;
        QLineEdit *m_kernelArgsLineEdit;

        QPushButton *m_kernelPathPushButton;
        QPushButton *m_initrdPushButton;

        // Methods
        void moveUpButton();
        void moveDownButton();
};

#endif // MACHINECONFIGBOOT_H

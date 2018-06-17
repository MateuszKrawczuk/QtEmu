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

// Qt

// Local
#include "machineconfigboot.h"

MachineConfigBoot::MachineConfigBoot(Machine *machine,
                                     QWidget *parent) : QWidget(parent) {

    m_bootMenuCheckBox = new QCheckBox();
    m_bootMenuCheckBox -> setText(tr("Enable boot menu"));

    m_moveUpToolButton = new QToolButton();
    m_moveUpToolButton -> setArrowType(Qt::UpArrow);

    m_moveDownToolButton = new QToolButton();
    m_moveDownToolButton -> setArrowType(Qt::DownArrow);

    m_floppyTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_floppyTreeItem -> setIcon(0, QIcon::fromTheme("media-floppy",
                                                 QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_floppyTreeItem -> setText(0, tr("Floppy"));
    m_floppyTreeItem -> setData(0, Qt::UserRole, "a");
    m_floppyTreeItem -> setCheckState(0, Qt::Unchecked);

    m_CDROMTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_CDROMTreeItem -> setIcon(0, QIcon::fromTheme("media-optical",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_CDROMTreeItem -> setText(0, tr("CDROM"));
    m_CDROMTreeItem -> setData(0, Qt::UserRole, "d");
    m_CDROMTreeItem -> setCheckState(0, Qt::Unchecked);

    m_hardDiskTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_hardDiskTreeItem -> setIcon(0, QIcon::fromTheme("drive-harddisk",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_hardDiskTreeItem -> setText(0, tr("Hard Disk"));
    m_hardDiskTreeItem -> setData(0, Qt::UserRole, "c");
    m_hardDiskTreeItem -> setCheckState(0, Qt::Unchecked);

    m_networkTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_networkTreeItem -> setIcon(0, QIcon::fromTheme("network-wired",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_networkTreeItem -> setText(0, tr("Network"));
    m_networkTreeItem -> setData(0, Qt::UserRole, "n");
    m_networkTreeItem -> setCheckState(0, Qt::Unchecked);

    m_bootTree = new QTreeWidget();
    m_bootTree -> setMaximumHeight(120);
    m_bootTree -> setMaximumWidth(150);
    m_bootTree -> setColumnCount(1);
    m_bootTree -> setHeaderHidden(true);
    m_bootTree -> setRootIsDecorated(false);
    m_bootTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_bootTree -> insertTopLevelItem(0, m_floppyTreeItem);
    m_bootTree -> insertTopLevelItem(1, m_CDROMTreeItem);
    m_bootTree -> insertTopLevelItem(2, m_hardDiskTreeItem);
    m_bootTree -> insertTopLevelItem(3, m_networkTreeItem);

    m_kernelBootCheckBox = new QCheckBox();
    m_kernelBootCheckBox -> setText(tr("Enable direct kernel boot"));

    m_kernelPathLabel = new QLabel(tr("Kernel path") + ":");
    m_initrdLabel = new QLabel(tr("Initrd path") + ":");
    m_kernelArgsLabel = new QLabel(tr("Kernel args") + ":");

    m_kernelPathLineEdit = new QLineEdit();
    m_kernelPathLineEdit -> setPlaceholderText("/boot/vmlinuz-linux");
    m_initredLineEdit = new QLineEdit();
    m_initredLineEdit -> setPlaceholderText("/boot/initramfs-linux.img");
    m_kernelArgsLineEdit = new QLineEdit();

    m_kernelPathPushButton = new QPushButton();
    m_kernelPathPushButton -> setIcon(QIcon::fromTheme("folder-symbolic",
                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    m_initrdPushButton = new QPushButton();
    m_initrdPushButton -> setIcon(QIcon::fromTheme("folder-symbolic",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));

    m_bootTreeLayout = new QHBoxLayout();
    m_bootTreeLayout -> setAlignment(Qt::AlignTop);
    m_bootTreeLayout -> setSpacing(5);
    m_bootTreeLayout -> addWidget(m_bootTree);
    m_bootTreeLayout -> addWidget(m_moveUpToolButton);
    m_bootTreeLayout -> addWidget(m_moveDownToolButton);

    m_kernelLayout = new QGridLayout();
    m_kernelLayout -> setSpacing(5);
    m_kernelLayout -> addWidget(m_kernelPathLabel,      0, 0, 1, 1);
    m_kernelLayout -> addWidget(m_kernelPathLineEdit,   0, 1, 1, 1);
    m_kernelLayout -> addWidget(m_kernelPathPushButton, 0, 2, 1, 1);
    m_kernelLayout -> addWidget(m_initrdLabel,          1, 0, 1, 1);
    m_kernelLayout -> addWidget(m_initredLineEdit,      1, 1, 1, 1);
    m_kernelLayout -> addWidget(m_initrdPushButton,     1, 2, 1, 1);
    m_kernelLayout -> addWidget(m_kernelArgsLabel,      2, 0, 1, 1);
    m_kernelLayout -> addWidget(m_kernelArgsLineEdit,   2, 1, 1, 1);

    m_bootPageLayout = new QVBoxLayout();
    m_bootPageLayout -> setAlignment(Qt::AlignTop);
    m_bootPageLayout -> addWidget(m_bootMenuCheckBox);
    m_bootPageLayout -> addItem(m_bootTreeLayout);
    m_bootPageLayout -> addWidget(m_kernelBootCheckBox);
    m_bootPageLayout -> addItem(m_kernelLayout);

    m_bootPageWidget = new QWidget(this);
    m_bootPageWidget -> setLayout(m_bootPageLayout);

    qDebug() << "MachineConfigBoot created";
}

MachineConfigBoot::~MachineConfigBoot() {
    qDebug() << "MachineConfigBoot destroyed";
}

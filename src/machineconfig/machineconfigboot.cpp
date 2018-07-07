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

// Local
#include "machineconfigboot.h"

MachineConfigBoot::MachineConfigBoot(Machine *machine,
                                     QWidget *parent) : QWidget(parent) {

    m_bootMenuCheckBox = new QCheckBox();
    m_bootMenuCheckBox -> setText(tr("Enable boot menu"));

    m_moveUpToolButton = new QToolButton();
    m_moveUpToolButton -> setArrowType(Qt::UpArrow);
    connect(m_moveUpToolButton, &QAbstractButton::clicked,
            this, &MachineConfigBoot::moveUpButton);

    m_moveDownToolButton = new QToolButton();
    m_moveDownToolButton -> setArrowType(Qt::DownArrow);
    connect(m_moveDownToolButton, &QAbstractButton::clicked,
            this, &MachineConfigBoot::moveDownButton);

    /*QStringList bootOrderList;

    QMap<QString, QString> mediaDevices = SystemUtils::getMediaDevices();
    QHash
    QMapIterator<QString, QString> i(bootOrderVariantHash);
    while(i.hasNext()) {
        i.next();
        bootOrderList.append(i.value());
    }*/






    m_bootTree = new QTreeWidget();
    m_bootTree -> setMaximumHeight(120);
    m_bootTree -> setMaximumWidth(150);
    m_bootTree -> setColumnCount(1);
    m_bootTree -> setHeaderHidden(true);
    m_bootTree -> setRootIsDecorated(false);
    m_bootTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

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

void MachineConfigBoot::moveUpButton() {

    int index = this -> m_bootTree -> currentIndex().row();
    if( index < 1 || index > this -> m_bootTree -> topLevelItemCount() ) {
        return;
    }

    QTreeWidgetItem *item = this -> m_bootTree -> takeTopLevelItem( index );
    this -> m_bootTree -> insertTopLevelItem( index - 1, item );

    this -> m_bootTree -> setCurrentItem( item );
}

void MachineConfigBoot::moveDownButton() {

    int index = this -> m_bootTree -> currentIndex().row();
    if( index < 0 || index > this -> m_bootTree -> topLevelItemCount() - 2 ) {
        return;
    }

    QTreeWidgetItem *item = this -> m_bootTree -> takeTopLevelItem( index );
    this -> m_bootTree -> insertTopLevelItem( index + 1, item );

    this -> m_bootTree -> setCurrentItem( item );
}

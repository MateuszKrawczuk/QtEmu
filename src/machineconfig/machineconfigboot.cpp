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

    bool enableFields = true;

    if (machine -> getState() != Machine::Stopped) {
        enableFields = false;
    }

    m_bootMenuCheckBox = new QCheckBox();
    m_bootMenuCheckBox -> setEnabled(enableFields);
    m_bootMenuCheckBox -> setText(tr("Enable boot menu"));

    m_moveUpToolButton = new QToolButton();
    m_moveUpToolButton -> setArrowType(Qt::UpArrow);
    m_moveUpToolButton -> setEnabled(enableFields);
    connect(m_moveUpToolButton, &QAbstractButton::clicked,
            this, &MachineConfigBoot::moveUpButton);

    m_moveDownToolButton = new QToolButton();
    m_moveDownToolButton -> setArrowType(Qt::DownArrow);
    m_moveDownToolButton -> setEnabled(enableFields);
    connect(m_moveDownToolButton, &QAbstractButton::clicked,
            this, &MachineConfigBoot::moveDownButton);

    Boot boot = machine -> getMachineBoot();
    QMap<int, QString> bootOrder = boot.getBootOrder();

    QSet<QString> bootSet;
    int maxBootOrderInt = bootOrder.size();
    QMapIterator<int, QString> i(bootOrder);
    while (i.hasNext()) {
        i.next();
        bootSet.insert(i.value());
    }

    QMap<QString, QString> mediaDevicesMap = SystemUtils::getMediaDevices();
    QMapIterator<QString, QString> j(mediaDevicesMap);
    while (j.hasNext()) {
        j.next();
        if ( ! bootSet.contains(j.value())) {
            bootOrder.insert(maxBootOrderInt, j.value());
            maxBootOrderInt++;
        }
    }

    m_bootTree = new QTreeWidget();
    m_bootTree -> setMaximumHeight(120);
    m_bootTree -> setMaximumWidth(150);
    m_bootTree -> setColumnCount(1);
    m_bootTree -> setHeaderHidden(true);
    m_bootTree -> setRootIsDecorated(false);
    m_bootTree -> setEnabled(enableFields);
    m_bootTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QMapIterator<int, QString> k(bootOrder);
    while (k.hasNext()) {
        k.next();
        m_treeItem = new QTreeWidgetItem(this -> m_bootTree, QTreeWidgetItem::Type);
        m_treeItem -> setText(0, k.value());
        m_treeItem -> setData(0, Qt::UserRole, mediaDevicesMap.key(k.value()));
        if (bootSet.contains(k.value())) {
            m_treeItem -> setCheckState(0, Qt::Checked);
        } else {
            m_treeItem -> setCheckState(0, Qt::Unchecked);
        }
    }

    m_kernelBootCheckBox = new QCheckBox();
    m_kernelBootCheckBox -> setEnabled(enableFields);
    m_kernelBootCheckBox -> setText(tr("Enable direct kernel boot"));
    connect(m_kernelBootCheckBox, &QAbstractButton::toggled,
            this, &MachineConfigBoot::selectEnableKernelBoot);

    m_kernelPathLabel = new QLabel(tr("Kernel path") + ":");
    m_initrdLabel = new QLabel(tr("Initrd path") + ":");
    m_kernelArgsLabel = new QLabel(tr("Kernel args") + ":");

    m_kernelPathLineEdit = new QLineEdit();
    m_kernelPathLineEdit -> setPlaceholderText("/boot/vmlinuz-linux");
    m_kernelPathLineEdit -> setEnabled(enableFields);
    m_initredLineEdit = new QLineEdit();
    m_initredLineEdit -> setPlaceholderText("/boot/initramfs-linux.img");
    m_initredLineEdit -> setEnabled(enableFields);
    m_kernelArgsLineEdit = new QLineEdit();
    m_kernelArgsLineEdit -> setEnabled(enableFields);

    m_kernelPathPushButton = new QPushButton();
    m_kernelPathPushButton -> setEnabled(enableFields);
    m_kernelPathPushButton -> setIcon(QIcon::fromTheme("folder-symbolic",
                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_kernelPathPushButton, &QAbstractButton::clicked,
            this, &MachineConfigBoot::setKernelPath);

    m_initrdPushButton = new QPushButton();
    m_initrdPushButton -> setEnabled(enableFields);
    m_initrdPushButton -> setIcon(QIcon::fromTheme("folder-symbolic",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_initrdPushButton, &QAbstractButton::clicked,
            this, &MachineConfigBoot::setInitrdPath);

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

    this -> selectEnableKernelBoot(boot.kernelBootEnabled());

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

void MachineConfigBoot::selectEnableKernelBoot(bool enableKernelBoot) {

    this -> m_kernelPathLineEdit   -> setEnabled(enableKernelBoot);
    this -> m_initredLineEdit      -> setEnabled(enableKernelBoot);
    this -> m_kernelArgsLineEdit   -> setEnabled(enableKernelBoot);
    this -> m_kernelPathPushButton -> setEnabled(enableKernelBoot);
    this -> m_initrdPushButton     -> setEnabled(enableKernelBoot);
}

void MachineConfigBoot::setKernelPath(){

    QString kernelPath = QFileDialog::getOpenFileName(this, tr("Select kernel path"),
                                                      QDir::homePath());

    if ( ! kernelPath.isEmpty() ) {
        this -> m_kernelPathLineEdit -> setText(QDir::toNativeSeparators(kernelPath));
    }

}

void MachineConfigBoot::setInitrdPath(){

    QString initrdPath = QFileDialog::getOpenFileName(this, tr("Select initrd path"),
                                                      QDir::homePath());

    if ( ! initrdPath.isEmpty() ) {
        this -> m_initredLineEdit -> setText(QDir::toNativeSeparators(initrdPath));
    }

}

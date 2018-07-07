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
#include "machineconfigaccel.h"

MachineConfigAccel::MachineConfigAccel(Machine *machine,
                                       QWidget *parent) : QWidget(parent) {

    bool enableFields = true;

    if (machine -> getState() != Machine::Stopped) {
        enableFields = false;
    }

    m_moveUpAccelToolButton = new QToolButton();
    m_moveUpAccelToolButton -> setArrowType(Qt::UpArrow);
    m_moveUpAccelToolButton -> setEnabled(enableFields);
    connect(m_moveUpAccelToolButton, &QAbstractButton::clicked,
            this, &MachineConfigAccel::moveUpButton);

    m_moveDownAccelToolButton = new QToolButton();
    m_moveDownAccelToolButton -> setArrowType(Qt::DownArrow);
    m_moveDownAccelToolButton -> setEnabled(enableFields);
    connect(m_moveDownAccelToolButton, &QAbstractButton::clicked,
            this, &MachineConfigAccel::moveDownButton);

    QStringList accelList = machine -> getAccelerator();

    QHash<QString, QString> accelHash = SystemUtils::getAccelerators();
    QHashIterator<QString, QString> i(accelHash);
    while (i.hasNext()) {
        i.next();
        if ( ! accelList.contains(i.key())) {
            accelList.append(i.key());
        }
    }

    m_acceleratorTree = new QTreeWidget();
    m_acceleratorTree -> setMaximumHeight(150);
    m_acceleratorTree -> setMaximumWidth(200);
    m_acceleratorTree -> setColumnCount(1);
    m_acceleratorTree -> setHeaderHidden(true);
    m_acceleratorTree -> setRootIsDecorated(false);
    m_acceleratorTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_acceleratorTree -> setEnabled(enableFields);

    for(int i = 0; i < accelList.size(); ++i) {
        m_treeItem = new QTreeWidgetItem(this -> m_acceleratorTree, QTreeWidgetItem::Type);
        m_treeItem -> setText(0, accelHash.value(accelList.at(i)));
        m_treeItem -> setData(0, Qt::UserRole, accelList.at(i));
        if (machine -> getAccelerator().contains(accelList.at(i))) {
            m_treeItem -> setCheckState(0, Qt::Checked);
        } else {
            m_treeItem -> setCheckState(0, Qt::Unchecked);
        }
    }

    m_accelTreeLayout = new QHBoxLayout();
    m_accelTreeLayout -> setAlignment(Qt::AlignTop);
    m_accelTreeLayout -> setSpacing(5);
    m_accelTreeLayout -> addWidget(m_acceleratorTree);
    m_accelTreeLayout -> addWidget(m_moveUpAccelToolButton);
    m_accelTreeLayout -> addWidget(m_moveDownAccelToolButton);

    m_acceleratorLayout = new QVBoxLayout();
    m_acceleratorLayout -> setAlignment(Qt::AlignTop);
    m_acceleratorLayout -> addItem(m_accelTreeLayout);

    m_acceleratorPageWidget = new QWidget();
    m_acceleratorPageWidget -> setLayout(m_acceleratorLayout);

    qDebug() << "MachineConfigAccel created";
}

MachineConfigAccel::~MachineConfigAccel() {
    qDebug() << "MachineConfigAccel destroyed";
}

void MachineConfigAccel::moveUpButton() {

    int index = this -> m_acceleratorTree -> currentIndex().row();
    if( index < 1 || index > this -> m_acceleratorTree -> topLevelItemCount() ) {
        return;
    }

    QTreeWidgetItem *item = this -> m_acceleratorTree -> takeTopLevelItem( index );
    this -> m_acceleratorTree -> insertTopLevelItem( index - 1, item );

    this -> m_acceleratorTree -> setCurrentItem( item );
}

void MachineConfigAccel::moveDownButton() {

    int index = this -> m_acceleratorTree -> currentIndex().row();
    if( index < 0 || index > this -> m_acceleratorTree -> topLevelItemCount() - 2 ) {
        return;
    }

    QTreeWidgetItem *item = this -> m_acceleratorTree ->takeTopLevelItem( index );
    this -> m_acceleratorTree -> insertTopLevelItem( index + 1, item );

    this -> m_acceleratorTree -> setCurrentItem( item );
}

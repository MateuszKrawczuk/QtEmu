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
#include "machineconfigaudio.h"

MachineConfigAudio::MachineConfigAudio(Machine *machine,
                                       QWidget *parent) : QWidget(parent) {

    bool enableFields = true;

    if (machine -> getState() != Machine::Stopped) {
        enableFields = false;
    }

    m_moveUpAudioToolButton = new QToolButton();
    m_moveUpAudioToolButton -> setArrowType(Qt::UpArrow);
    m_moveUpAudioToolButton -> setEnabled(enableFields);
    connect(m_moveUpAudioToolButton, &QAbstractButton::clicked,
            this, &MachineConfigAudio::moveUpButton);

    m_moveDownAudioToolButton = new QToolButton();
    m_moveDownAudioToolButton -> setArrowType(Qt::DownArrow);
    m_moveDownAudioToolButton -> setEnabled(enableFields);
    connect(m_moveDownAudioToolButton, &QAbstractButton::clicked,
            this, &MachineConfigAudio::moveDownButton);

    QStringList audioList = machine -> getAudio();

    QHash<QString, QString> audioHash = SystemUtils::getSoundCards();
    QHashIterator<QString, QString> i(audioHash);
    while (i.hasNext()) {
        i.next();
        if ( ! audioList.contains(i.key())) {
            audioList.append(i.key());
        }
    }

    m_audioTree = new QTreeWidget();
    m_audioTree -> setMaximumHeight(200);
    m_audioTree -> setMaximumWidth(250);
    m_audioTree -> setColumnCount(1);
    m_audioTree -> setHeaderHidden(true);
    m_audioTree -> setRootIsDecorated(false);
    m_audioTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_audioTree -> setEnabled(enableFields);

    for(int i = 0; i < audioList.size(); ++i) {
        m_treeItem = new QTreeWidgetItem(this -> m_audioTree, QTreeWidgetItem::Type);
        m_treeItem -> setText(0, audioHash.value(audioList.at(i)));
        m_treeItem -> setData(0, Qt::UserRole, audioList.at(i));
        if (machine -> getAudio().contains(audioList.at(i))) {
            m_treeItem -> setCheckState(0, Qt::Checked);
        } else {
            m_treeItem -> setCheckState(0, Qt::Unchecked);
        }
    }

    m_audioTreeLayout = new QHBoxLayout();
    m_audioTreeLayout -> setAlignment(Qt::AlignTop);
    m_audioTreeLayout -> setSpacing(5);
    m_audioTreeLayout -> addWidget(m_audioTree);
    m_audioTreeLayout -> addWidget(m_moveUpAudioToolButton);
    m_audioTreeLayout -> addWidget(m_moveDownAudioToolButton);

    m_hostSoundSystemLabel = new QLabel(tr("Host sound system") + ":");

    m_hostSoundSystemComboBox = new QComboBox();
    m_hostSoundSystemComboBox -> setEnabled(enableFields);
    m_hostSoundSystemComboBox -> addItem("alsa");
    m_hostSoundSystemComboBox -> addItem("sdl");
    m_hostSoundSystemComboBox -> addItem("pa");
    m_hostSoundSystemComboBox -> addItem("spice");
    m_hostSoundSystemComboBox -> addItem("wav");
    m_hostSoundSystemComboBox -> addItem("none");

    m_hostSoundSystemLayout = new QHBoxLayout();
    m_hostSoundSystemLayout -> setAlignment(Qt::AlignLeft);
    m_hostSoundSystemLayout -> setSpacing(5);
    m_hostSoundSystemLayout -> addWidget(m_hostSoundSystemLabel);
    m_hostSoundSystemLayout -> addWidget(m_hostSoundSystemComboBox);

    m_audioLayout = new QVBoxLayout();
    m_audioLayout -> setAlignment(Qt::AlignTop);
    m_audioLayout -> addItem(m_audioTreeLayout);
    m_audioLayout -> addItem(m_hostSoundSystemLayout);

    m_audioPageWidget = new QWidget();
    m_audioPageWidget -> setLayout(m_audioLayout);

    qDebug() << "MachineConfigAudio created";
}

MachineConfigAudio::~MachineConfigAudio() {
    qDebug() << "MachineConfigAudio destroyed";
}

void MachineConfigAudio::moveUpButton() {

    int index = this -> m_audioTree -> currentIndex().row();
    if( index < 1 || index > this -> m_audioTree -> topLevelItemCount() ) {
        return;
    }

    QTreeWidgetItem *item = this -> m_audioTree -> takeTopLevelItem( index );
    this -> m_audioTree -> insertTopLevelItem( index - 1, item );

    this -> m_audioTree -> setCurrentItem( item );
}

void MachineConfigAudio::moveDownButton() {

    int index = this -> m_audioTree -> currentIndex().row();
    if( index < 0 || index > this -> m_audioTree -> topLevelItemCount() - 2 ) {
        return;
    }

    QTreeWidgetItem *item = this -> m_audioTree -> takeTopLevelItem( index );
    this -> m_audioTree -> insertTopLevelItem( index + 1, item );

    this -> m_audioTree -> setCurrentItem( item );
}

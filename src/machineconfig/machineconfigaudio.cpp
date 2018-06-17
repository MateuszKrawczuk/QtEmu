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
#include "machineconfigaudio.h"

MachineConfigAudio::MachineConfigAudio(Machine *machine,
                                       QWidget *parent) : QWidget(parent) {

    m_moveUpAudioToolButton = new QToolButton();
    m_moveUpAudioToolButton -> setArrowType(Qt::UpArrow);

    m_moveDownAudioToolButton = new QToolButton();
    m_moveDownAudioToolButton -> setArrowType(Qt::DownArrow);

    // TODO: Fix that...thing... do it into a for
    m_creativeTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_creativeTreeItem -> setText(0, tr("Creative Sound Blaster 16"));
    m_creativeTreeItem -> setData(0, Qt::UserRole, "sb16");
    m_creativeTreeItem -> setCheckState(0, Qt::Unchecked);

    m_intelAC97TreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_intelAC97TreeItem -> setText(0, tr("Intel AC97(82801AA)"));
    m_intelAC97TreeItem -> setData(0, Qt::UserRole, "ac97");
    m_intelAC97TreeItem -> setCheckState(0, Qt::Unchecked);

    m_gravisTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_gravisTreeItem -> setText(0, tr("Gravis Ultrasound GF1"));
    m_gravisTreeItem -> setData(0, Qt::UserRole, "gus");
    m_gravisTreeItem -> setCheckState(0, Qt::Unchecked);

    m_intelHDTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_intelHDTreeItem -> setText(0, tr("Intel HD Audio"));
    m_intelHDTreeItem -> setData(0, Qt::UserRole, "hda");
    m_intelHDTreeItem -> setCheckState(0, Qt::Unchecked);

    m_ensoniqTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_ensoniqTreeItem -> setText(0, tr("ENSONIQ AudioPCI ES1370"));
    m_ensoniqTreeItem -> setData(0, Qt::UserRole, "es1370");
    m_ensoniqTreeItem -> setCheckState(0, Qt::Unchecked);

    m_yamahaTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_yamahaTreeItem -> setText(0, tr("Yamaha YM3812"));
    m_yamahaTreeItem -> setData(0, Qt::UserRole, "adlib");
    m_yamahaTreeItem -> setCheckState(0, Qt::Unchecked);

    m_CS4231ATreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_CS4231ATreeItem -> setText(0, tr("CS4231A"));
    m_CS4231ATreeItem -> setData(0, Qt::UserRole, "cs4231a");
    m_CS4231ATreeItem -> setCheckState(0, Qt::Unchecked);

    m_speakerTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_speakerTreeItem -> setText(0, tr("PC Speaker"));
    m_speakerTreeItem -> setData(0, Qt::UserRole, "pcspk");
    m_speakerTreeItem -> setCheckState(0, Qt::Unchecked);

    m_audioTree = new QTreeWidget();
    m_audioTree -> setMaximumHeight(200);
    m_audioTree -> setMaximumWidth(250);
    m_audioTree -> setColumnCount(1);
    m_audioTree -> setHeaderHidden(true);
    m_audioTree -> setRootIsDecorated(false);
    m_audioTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_audioTree -> insertTopLevelItem(0, m_creativeTreeItem);
    m_audioTree -> insertTopLevelItem(1, m_intelAC97TreeItem);
    m_audioTree -> insertTopLevelItem(2, m_gravisTreeItem);
    m_audioTree -> insertTopLevelItem(3, m_intelHDTreeItem);
    m_audioTree -> insertTopLevelItem(4, m_ensoniqTreeItem);
    m_audioTree -> insertTopLevelItem(5, m_yamahaTreeItem);
    m_audioTree -> insertTopLevelItem(6, m_CS4231ATreeItem);
    m_audioTree -> insertTopLevelItem(7, m_speakerTreeItem);

    m_audioTreeLayout = new QHBoxLayout();
    m_audioTreeLayout -> setAlignment(Qt::AlignTop);
    m_audioTreeLayout -> setSpacing(5);
    m_audioTreeLayout -> addWidget(m_audioTree);
    m_audioTreeLayout -> addWidget(m_moveUpAudioToolButton);
    m_audioTreeLayout -> addWidget(m_moveDownAudioToolButton);

    m_hostSoundSystemLabel = new QLabel(tr("Host sound system") + ":");

    m_hostSoundSystemComboBox = new QComboBox();
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

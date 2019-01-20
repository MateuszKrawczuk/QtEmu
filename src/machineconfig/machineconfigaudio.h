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
#ifndef MACHINECONFIGAUDIO_H
#define MACHINECONFIGAUDIO_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTreeWidget>
#include <QComboBox>
#include <QLabel>

// Local
#include "../machine.h"
#include "../utils/systemutils.h"

class MachineConfigAudio : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigAudio(Machine *machine,
                                    QWidget *parent = nullptr);
        ~MachineConfigAudio();
        QWidget *m_audioPageWidget;

        // Methods
        void saveAudioData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_audioTreeLayout;
        QHBoxLayout *m_hostSoundSystemLayout;
        QVBoxLayout *m_audioLayout;

        QToolButton *m_moveUpAudioToolButton;
        QToolButton *m_moveDownAudioToolButton;

        QTreeWidget *m_audioTree;

        QTreeWidgetItem *m_treeItem;

        QTreeWidgetItem *m_creativeTreeItem;
        QTreeWidgetItem *m_intelAC97TreeItem;
        QTreeWidgetItem *m_gravisTreeItem;
        QTreeWidgetItem *m_intelHDTreeItem;
        QTreeWidgetItem *m_ensoniqTreeItem;
        QTreeWidgetItem *m_yamahaTreeItem;
        QTreeWidgetItem *m_CS4231ATreeItem;
        QTreeWidgetItem *m_speakerTreeItem;

        QLabel *m_hostSoundSystemLabel;

        QComboBox *m_hostSoundSystemComboBox;

        Machine *m_machine;

        // Methods
        void moveUpButton();
        void moveDownButton();
};
#endif // MACHINECONFIGAUDIO_H

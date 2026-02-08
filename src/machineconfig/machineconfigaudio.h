// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

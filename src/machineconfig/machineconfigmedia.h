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
#ifndef MACHINECONFIGMEDIA_H
#define MACHINECONFIGMEDIA_H

// Qt
#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QAction>
#include <QMenu>

// Local
#include "../machine.h"
#include "../qemu.h"
#include "../utils/newdiskwizard.h"
#include "../utils/systemutils.h"

class MachineConfigMedia : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigMedia(Machine *machine,
                                    QEMU *QEMUGlobalObject,
                                    QWidget *parent = nullptr);
        ~MachineConfigMedia();
        QWidget *m_mediaPageWidget;

        // Methods
        void saveMediaData();

    signals:

    public slots:

    private slots:
        void removeMedia(const QPoint &pos);
        void removeMediaFromTree();

    protected:

    private:
        QGridLayout *m_mediaPageLayout;
        QFormLayout *m_mediaDetailsLayout;
        QFormLayout *m_mediaOptionsLayout;
        QHBoxLayout *m_mediaAddLayout;

        QTreeWidget *m_mediaTree;
        QTreeWidgetItem *m_mediaItem;

        QLabel *m_mediaNameLabel;
        QLabel *m_mediaPathLabel;

        QGroupBox *m_mediaSettingsGroupBox;
        QGroupBox *m_mediaOptionsGroupBox;
        QGroupBox *m_mediaAddGroupBox;

        QComboBox *m_cacheComboBox;
        QComboBox *m_IOComboBox;

        QCheckBox *m_readOnlyMediaCheck;

        QPushButton *m_addFloppyPushButton;
        QPushButton *m_addHDDPushButton;
        QPushButton *m_addCDROMPushButton;

        QAction *m_removeMediaAction;

        QMenu *m_menu;

        QMessageBox *m_addHddDiskMessageBox;
        QMessageBox *m_addOpticalMessageBox;
        QMessageBox *m_addFloppyMessageBox;

        QMessageBox *m_maxHddDiskMessageBox;
        QMessageBox *m_maxOpticalMessageBox;
        QMessageBox *m_hdcOpticalConflictMessageBox;
        QMessageBox *m_maxFloppyMessageBox;

        QMap<QString, QString> *m_diskMap;
        QMap<QString, QString> *m_floppyMap;
        QMap<QString, QString> *m_cdromMap; // Yes, I know there's only one cdrom...

        Machine *m_machineOptions;
        QEMU *m_qemuGlobalObject;

        // Methods
        void fillDetailsSection();
        void addFloppyMedia();
        void addHddMedia();
        void addOpticalMedia();
        void fillMaps();
        void addMediaToTree(Media media);
        void removeInterface(const QString driveInterface);
        int countMedia();
};
#endif // MACHINECONFIGMEDIA_H

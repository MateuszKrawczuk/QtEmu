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

// Local
#include "../machine.h"

class MachineConfigMedia : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigMedia(Machine *machine,
                                    QWidget *parent = 0);
        ~MachineConfigMedia();
        QWidget *m_mediaPageWidget;

    signals:

    public slots:

    private slots:

    protected:

    private:
        QGridLayout *m_mediaPageLayout;
        QFormLayout *m_mediaDetailsLayout;
        QFormLayout *m_mediaOptionsLayout;
        QHBoxLayout *m_mediaAddLayout;

        QTreeWidget *m_mediaTree;
        QTreeWidgetItem *m_mediaItem;

        QLabel *m_mediaPathLabel;
        QLabel *m_mediaSizeLabel;
        QLabel *m_mediaFormatLabel;

        QGroupBox *m_mediaSettingsGroupBox;
        QGroupBox *m_mediaOptionsGroupBox;
        QGroupBox *m_mediaAddGroupBox;

        QComboBox *m_cacheComboBox;
        QComboBox *m_IOComboBox;

        QCheckBox *m_readOnlyMediaCheck;

        QPushButton *m_addHDDPushButton;
        QPushButton *m_addCDROMPushButton;
};
#endif // MACHINECONFIGMEDIA_H

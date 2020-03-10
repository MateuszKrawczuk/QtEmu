/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef CONCLUSIONPAGE_H
#define CONCLUSIONPAGE_H

// Qt
#include <QWizard>
#include <QLabel>
#include <QGridLayout>
#include <QSettings>
#include <QDir>
#include <QListWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUuid>
#include <QFileInfo>

// Local
#include "../machine.h"
#include "../utils/logger.h"

class MachineConclusionPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineConclusionPage(Machine *machine,
                                       QListWidget *osListWidget,
                                       QEMU *QEMUGlobalObject,
                                       QWidget *parent = nullptr);
        ~MachineConclusionPage();

    signals:

    public slots:

    protected:

    private:
        QGridLayout *m_conclusionLayout;

        QLabel *m_machineDescLabel;
        QLabel *m_OSTypeDescLabel;
        QLabel *m_OSVersionDescLabel;
        QLabel *m_processorDescLabel;
        QLabel *m_graphicsDescLabel;
        QLabel *m_audioDescLabel;
        QLabel *m_RAMDescLabel;
        QLabel *m_acceleratorDescLabel;
        QLabel *m_diskDescLabel;

        QLabel *m_conclusionLabel;
        QLabel *m_machineNameLabel;
        QLabel *m_OSTypeLabel;
        QLabel *m_OSVersionLabel;
        QLabel *m_processorLabel;
        QLabel *m_graphicsLabel;
        QLabel *m_audioLabel;
        QLabel *m_RAMLabel;
        QLabel *m_acceleratorLabel;
        QLabel *m_diskLabel;

        QListWidget *m_osList;

        Machine *m_newMachine;

        QEMU *m_QEMUGlobalObject;

        // Methods
        void initializePage();
        bool validatePage();
        void insertVMList();
        void generateMachineFiles();
        void addMedia(const QString name,
                      const QString format,
                      const QString paths);
        void generateBoot();
};

#endif // CONCLUSIONPAGE_H

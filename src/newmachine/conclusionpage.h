// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef IMPORTMEDIAPAGE_H
#define IMPORTMEDIAPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QListWidget>

#include <QDebug>

// Local
#include "../machine.h"

class ImportMediaPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ImportMediaPage(Machine *machine,
                                 QListWidget *osListWidget,
                                 QWidget *parent = nullptr);
        ~ImportMediaPage();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_mediaLayout;
        QVBoxLayout *m_mainLayout;

        QTreeWidget *m_machineMediaTree;
        QTreeWidgetItem *m_mediaItem;

        QLabel *m_infoLabel;

        QListWidget *m_osList;

        Machine *m_machine;

        // Methods
        void initializePage();
        bool validatePage();
        void insertVMList();
};

#endif // IMPORTMEDIAPAGE_H

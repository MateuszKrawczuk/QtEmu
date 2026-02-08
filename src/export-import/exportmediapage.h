// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef EXPORTMEDIAPAGE_H
#define EXPORTMEDIAPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>

// Local
#include "../machine.h"

class ExportMediaPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ExportMediaPage(Machine *machine,
                                 QWidget *parent = nullptr);
        ~ExportMediaPage();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_mediaLayout;

        QTreeWidget *m_machineMediaTree;
        QTreeWidgetItem *m_mediaItem;

        QLabel *m_mediaTitleLabel;

        Machine *m_machineExport;

        bool validatePage();

};

#endif // EXPORTMEDIAPAGE_H

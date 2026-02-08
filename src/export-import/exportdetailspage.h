// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef EXPORTDETAILSPAGE_H
#define EXPORTDETAILSPAGE_H

// Qt
#include <QWizard>
#include <QGridLayout>
#include <QLabel>

// Local
#include "../machine.h"

class ExportDetailsPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ExportDetailsPage(Machine *machine,
                                   QWidget *parent = nullptr);
        ~ExportDetailsPage();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QGridLayout *m_mainLayout;

        QLabel *m_machineDescLabel;
        QLabel *m_OSTypeDescLabel;
        QLabel *m_OSVersionDescLabel;
        QLabel *m_processorDescLabel;
        QLabel *m_graphicsDescLabel;
        QLabel *m_audioDescLabel;
        QLabel *m_RAMDescLabel;
        QLabel *m_acceleratorDescLabel;

        QLabel *m_conclusionLabel;
        QLabel *m_machineNameLabel;
        QLabel *m_OSTypeLabel;
        QLabel *m_OSVersionLabel;
        QLabel *m_processorLabel;
        QLabel *m_graphicsLabel;
        QLabel *m_audioLabel;
        QLabel *m_RAMLabel;
        QLabel *m_acceleratorLabel;

};

#endif // EXPORTDETAILSPAGE_H

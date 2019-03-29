/*
 * This file is part of QtEmu project.
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

#ifndef IMPORTDETAILSPAGE_H
#define IMPORTDETAILSPAGE_H

// Qt
#include <QWizard>
#include <QGridLayout>
#include <QLabel>

#include <QDebug>

// Local
#include "../machine.h"
#include "../machineutils.h"

class ImportDetailsPage : public QWizardPage {
    Q_OBJECT

    public:
        explicit ImportDetailsPage(Machine *machine,
                                   QWidget *parent = nullptr);
        ~ImportDetailsPage();

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

        Machine *m_machine;

        // Methods
        void initializePage();
};

#endif // IMPORTDETAILSPAGE_H

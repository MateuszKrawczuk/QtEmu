/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include <QFormLayout>

// Local
#include "../machine.h"

class MachineConclusionPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineConclusionPage(Machine *machine,
                                       QWidget *parent = 0);
        ~MachineConclusionPage();

    signals:

    public slots:

    protected:

    private:
        QFormLayout *conclusionLayout;

        QLabel *conclusionCPULabel;

        Machine *newMachine;

};


#endif // CONCLUSIONPAGE_H

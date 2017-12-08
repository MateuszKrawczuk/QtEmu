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

#ifndef ACCELERATORPAGE_H
#define ACCELERATORPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QRadioButton>
#include <QLabel>

// Local
#include "../machine.h"

class MachineAcceleratorPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineAcceleratorPage(Machine *machine,
                                        QWidget *parent = 0);
        ~MachineAcceleratorPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *acceleratorLayout;

        QTabWidget *acceleratorTabWidget;

        Machine *newMachine;
};

class KVMTab: public QWidget {
    Q_OBJECT

    public:
        explicit KVMTab(Machine *machine,
                        QWidget *parent = 0);
        ~KVMTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *kvmLayout;

        QRadioButton *kvmRadioButton;

        QLabel *kvmDescriptionLabel;
        QLabel *kvmURLLabel;

        Machine *newMachine;
};

class XENTab: public QWidget {
    Q_OBJECT

    public:
        explicit XENTab(Machine *machine,
                        QWidget *parent = 0);
        ~XENTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *xenLayout;

        QRadioButton *xenRadioButton;

        QLabel *xenDescriptionLabel;
        QLabel *xenURLLabel;

        Machine *newMachine;
};

class TCGTab: public QWidget {
    Q_OBJECT

    public:
        explicit TCGTab(Machine *machine,
                        QWidget *parent = 0);
        ~TCGTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *tcgLayout;

        QRadioButton *tcgRadioButton;

        QLabel *tcgDescriptionLabel;
        QLabel *tcgURLLabel;

        Machine *newMachine;
};

class HAXMTab: public QWidget {
    Q_OBJECT

    public:
        explicit HAXMTab(Machine *machine,
                         QWidget *parent = 0);
        ~HAXMTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *haxmLayout;

        QRadioButton *haxmRadioButton;

        QLabel *haxmDescriptionLabel;
        QLabel *haxmURLLabel;

        Machine *newMachine;
};

#endif // ACCELERATORPAGE_H

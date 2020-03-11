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

#ifndef ACCELERATORPAGE_H
#define ACCELERATORPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QLabel>

// Local
#include "../machine.h"

class MachineAcceleratorPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineAcceleratorPage(Machine *machine,
                                        QWidget *parent = nullptr);
        ~MachineAcceleratorPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_acceleratorLayout;

        QTabWidget *m_acceleratorTabWidget;

        Machine *m_newMachine;
};

class KVMTab: public QWidget {
    Q_OBJECT

    public:
        explicit KVMTab(Machine *machine,
                        QWidget *parent = nullptr);
        ~KVMTab();
    signals:

    public slots:
        void addKVMAccelerator(bool kvmAccelerator);

    protected:

    private:
        QVBoxLayout *m_kvmLayout;

        QCheckBox *m_kvmCheck;

        QLabel *m_kvmDescriptionLabel;
        QLabel *m_kvmURLLabel;

        Machine *m_newMachine;
};

class XENTab: public QWidget {
    Q_OBJECT

    public:
        explicit XENTab(Machine *machine,
                        QWidget *parent = nullptr);
        ~XENTab();
    signals:

    public slots:
        void addXENAccelerator(bool xenAccelerator);

    protected:

    private:
        QVBoxLayout *m_xenLayout;

        QCheckBox *m_xenCheck;

        QLabel *m_xenDescriptionLabel;
        QLabel *m_xenURLLabel;

        Machine *m_newMachine;
};

class TCGTab: public QWidget {
    Q_OBJECT

    public:
        explicit TCGTab(Machine *machine,
                        QWidget *parent = nullptr);
        ~TCGTab();
    signals:

    public slots:
        void addTCGAccelerator(bool tcgAccelerator);

    protected:

    private:
        QVBoxLayout *m_tcgLayout;

        QCheckBox *m_tcgCheck;

        QLabel *m_tcgDescriptionLabel;
        QLabel *m_tcgURLLabel;

        Machine *m_newMachine;
};

class HAXMTab: public QWidget {
    Q_OBJECT

    public:
        explicit HAXMTab(Machine *machine,
                         QWidget *parent = nullptr);
        ~HAXMTab();
    signals:

    public slots:
        void addHAXAccelerator(bool haxAccelerator);

    protected:

    private:
        QVBoxLayout *m_haxmLayout;

        QCheckBox *m_haxmCheck;

        QLabel *m_haxmDescriptionLabel;
        QLabel *m_haxmURLLabel;

        Machine *m_newMachine;
};

class HVFTab: public QWidget {
    Q_OBJECT

    public:
        explicit HVFTab(Machine *machine,
                        QWidget *parent = nullptr);
        ~HVFTab();
    signals:

    public slots:
        void addHVFAccelerator(bool hvfAccelerator);

    protected:

    private:
        QVBoxLayout *m_hvfLayout;

        QCheckBox *m_hvfCheck;

        QLabel *m_hvfDescriptionLabel;
        QLabel *m_hvfURLLabel;

        Machine *m_newMachine;
};

class WHPXTab: public QWidget {
    Q_OBJECT

    public:
        explicit WHPXTab(Machine *machine,
                         QWidget *parent = nullptr);

        ~WHPXTab();
    signals:

    public slots:
        void addWHPXAccelerator(bool whpxAccelerator);

    protected:

    private:
        QVBoxLayout *m_whpxLayout;

        QCheckBox *m_whpxCheck;

        QLabel *m_whpxDescriptionLabel;
        QLabel *m_whpxURLLabel;

        Machine *m_newMachine;
};

#endif // ACCELERATORPAGE_H

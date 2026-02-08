// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEMORYPAGE_H
#define MEMORYPAGE_H

// Qt
#include <QWizard>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>

// Local
#include <QGroupBox>

#include "../machine.h"
#include "../utils/systemutils.h"

class MachineMemoryPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineMemoryPage(Machine *machine,
                                   QWidget *parent = nullptr);
        ~MachineMemoryPage();

    signals:

    public slots:

    protected:

    private:
        bool validatePage();

        QGridLayout *m_machineMemoryLayout;

        QSpinBox *m_memorySpinBox;
        QSlider *m_memorySlider;

        QLabel *m_descriptionMemoryLabel;
        QLabel *m_spinBoxMemoryLabel;
        QLabel *m_minMemoryLabel;
        QLabel *m_maxMemorylabel;
        QGroupBox *m_memorySettings;

        Machine *m_newMachine;
};

#endif // MEMORYPAGE_H

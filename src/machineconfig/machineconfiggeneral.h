// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGGENERAL_H
#define MACHINECONFIGGENERAL_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QLineEdit>

// Local
#include "../machine.h"
#include "machineconfiggeneraltabs.h"

class MachineConfigGeneral : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigGeneral(Machine *machine,
                                      QWidget *parent = nullptr);
        ~MachineConfigGeneral();

        QWidget *m_generalPageWidget;

        // Methods
        void saveGeneralData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QTabWidget *m_generalTabWidget;
        QVBoxLayout *m_generalPageLayout;

        Machine *m_machine;
        BasicTab *m_basicTab;
        DescriptionTab *m_descriptionTab;
};

#endif // MACHINECONFIGGENERAL_H

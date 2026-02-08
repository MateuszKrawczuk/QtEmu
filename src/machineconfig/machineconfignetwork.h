// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGNETWORK_H
#define MACHINECONFIGNETWORK_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>

// Local
#include "../machine.h"

class MachineConfigNetwork : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigNetwork(Machine *machine,
                                      QWidget *parent = nullptr);
        ~MachineConfigNetwork();
        QWidget *m_networkPageWidget;

        // Methods
        void saveNetworkData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QVBoxLayout *m_networkLayout;
        QVBoxLayout *m_networkMainLayout;

        QGroupBox *m_machineNetworkGroup;

        QRadioButton *m_withNetworkRadio;
        QRadioButton *m_withoutNetworkRadio;

        Machine *m_machine;

};
#endif // MACHINECONFIGNETWORK_H

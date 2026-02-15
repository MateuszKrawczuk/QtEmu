// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGNETWORK_H
#define MACHINECONFIGNETWORK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "../machine.h"

class MachineConfigNetwork : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigNetwork(Machine *machine,
                                      QWidget *parent = nullptr);
        ~MachineConfigNetwork();
        QWidget *m_networkPageWidget;

        void saveNetworkData();

    signals:

    public slots:

    private slots:
        void onAddAdapterClicked();
        void onEditAdapterClicked();
        void onRemoveAdapterClicked();
        void onAdapterSelectionChanged();

    private:
        void setupUI();
        void loadAdapters();
        void updateTable();
        QString backendToString(NetworkBackend backend);
        QString nicModelToString(NicModel model);

        QVBoxLayout *m_networkLayout;
        QVBoxLayout *m_networkMainLayout;

        QGroupBox *m_machineNetworkGroup;
        QTableWidget *m_adapterTable;
        QPushButton *m_addAdapterBtn;
        QPushButton *m_editAdapterBtn;
        QPushButton *m_removeAdapterBtn;
        QLabel *m_infoLabel;

        Machine *m_machine;
        bool m_enableFields;
};

#endif

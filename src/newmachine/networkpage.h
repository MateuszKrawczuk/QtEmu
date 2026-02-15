// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QWizardPage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include "../machine.h"

class NetworkPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkPage(Machine *machine, QWidget *parent = nullptr);
    ~NetworkPage();

    void initializePage() override;
    bool validatePage() override;

private slots:
    void onEnableNetworkChanged(int state);
    void onAddAdapterClicked();
    void onEditAdapterClicked();
    void onRemoveAdapterClicked();
    void onAdapterSelectionChanged();

private:
    void setupUI();
    void updateTable();
    QString backendToString(NetworkBackend backend);
    QString nicModelToString(NicModel model);
    void addDefaultAdapter();

    Machine *m_machine;
    bool m_adaptersLoaded;

    QVBoxLayout *m_mainLayout;
    QCheckBox *m_enableNetworkCheck;
    QGroupBox *m_adaptersGroup;
    QTableWidget *m_adapterTable;
    QPushButton *m_addAdapterBtn;
    QPushButton *m_editAdapterBtn;
    QPushButton *m_removeAdapterBtn;
    QLabel *m_infoLabel;
};

#endif

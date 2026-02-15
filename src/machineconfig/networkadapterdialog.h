// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NETWORKADAPTERDIALOG_H
#define NETWORKADAPTERDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include "../networkadapter.h"

class NetworkAdapterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkAdapterDialog(NetworkAdapter *adapter, 
                                  bool enableFields = true,
                                  QWidget *parent = nullptr);
    ~NetworkAdapterDialog();

    void saveToAdapter();

private slots:
    void onBackendChanged(int index);
    void onGenerateMacClicked();
    void onAutoMacClicked();
    void onAddPortForwardClicked();
    void onEditPortForwardClicked();
    void onRemovePortForwardClicked();
    void onPortForwardSelectionChanged();

private:
    void setupUI();
    void loadFromAdapter();
    void updateBackendOptions();
    void updatePortForwardTable();
    QString formatPortForward(const PortForward &pf);

    NetworkAdapter *m_adapter;
    bool m_enableFields;
    QList<PortForward> m_portForwards;

    QLineEdit *m_idEdit;
    QComboBox *m_backendCombo;
    QComboBox *m_nicModelCombo;
    QLineEdit *m_macEdit;
    QPushButton *m_generateMacBtn;
    QCheckBox *m_autoMacCheck;
    QCheckBox *m_bootRomCheck;

    QGroupBox *m_userGroup;
    QTableWidget *m_portForwardTable;
    QPushButton *m_addPortForwardBtn;
    QPushButton *m_editPortForwardBtn;
    QPushButton *m_removePortForwardBtn;

    QGroupBox *m_bridgeGroup;
    QComboBox *m_bridgeCombo;
    QPushButton *m_refreshBridgeBtn;
    QPushButton *m_createBridgeBtn;

    QGroupBox *m_tapGroup;
    QLineEdit *m_tapDeviceEdit;

    QGroupBox *m_socketGroup;
    QLineEdit *m_socketListenEdit;
    QLineEdit *m_socketConnectEdit;

    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;
};

#endif

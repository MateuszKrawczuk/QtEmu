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
#include <QStackedWidget>
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

private:
    void setupUI();
    void loadFromAdapter();
    void updateBackendOptions();

    NetworkAdapter *m_adapter;
    bool m_enableFields;

    QLineEdit *m_idEdit;
    QComboBox *m_backendCombo;
    QComboBox *m_nicModelCombo;
    QLineEdit *m_macEdit;
    QPushButton *m_generateMacBtn;
    QCheckBox *m_autoMacCheck;
    QCheckBox *m_bootRomCheck;

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

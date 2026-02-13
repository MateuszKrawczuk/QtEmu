// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CREATEBRIDGEDIALOG_H
#define CREATEBRIDGEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

class CreateBridgeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateBridgeDialog(QWidget *parent = nullptr);
    ~CreateBridgeDialog();

    QString bridgeName() const;
    QString physicalInterface() const;

private slots:
    void onCreateClicked();
    void onRefreshInterfacesClicked();

private:
    void setupUI();
    void loadInterfaces();

    QLineEdit *m_bridgeNameEdit;
    QComboBox *m_interfaceCombo;
    QPushButton *m_refreshBtn;
    QLabel *m_infoLabel;
    QLabel *m_statusLabel;
    QPushButton *m_createBtn;
    QPushButton *m_cancelBtn;
};

#endif

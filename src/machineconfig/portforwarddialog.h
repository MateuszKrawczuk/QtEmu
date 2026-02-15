// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PORTFORWARDDIALOG_H
#define PORTFORWARDDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include "../networkadapter.h"

class PortForwardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortForwardDialog(QWidget *parent = nullptr);
    explicit PortForwardDialog(const PortForward &forward, QWidget *parent = nullptr);
    ~PortForwardDialog();

    PortForward getPortForward() const;

private slots:
    void validate();

private:
    void setupUI();

    QComboBox *m_protocolCombo;
    QSpinBox *m_hostPortSpin;
    QSpinBox *m_guestPortSpin;
    QLineEdit *m_hostAddressEdit;
    QLineEdit *m_guestAddressEdit;
    QPushButton *m_okBtn;
};

#endif

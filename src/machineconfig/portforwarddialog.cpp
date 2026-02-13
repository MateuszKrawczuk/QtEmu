// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "portforwarddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QRegularExpressionValidator>
#include <QLabel>
#include <QPushButton>

PortForwardDialog::PortForwardDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
}

PortForwardDialog::PortForwardDialog(const PortForward &forward, QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    
    int protocolIndex = m_protocolCombo->findText(forward.protocol);
    if (protocolIndex >= 0) {
        m_protocolCombo->setCurrentIndex(protocolIndex);
    }
    m_hostPortSpin->setValue(forward.hostPort);
    m_guestPortSpin->setValue(forward.guestPort);
    m_hostAddressEdit->setText(forward.hostAddress);
    m_guestAddressEdit->setText(forward.guestAddress);
}

PortForwardDialog::~PortForwardDialog()
{
}

void PortForwardDialog::setupUI()
{
    setWindowTitle(tr("Port Forwarding Rule"));
    setMinimumWidth(350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    m_protocolCombo = new QComboBox(this);
    m_protocolCombo->addItem(QStringLiteral("tcp"));
    m_protocolCombo->addItem(QStringLiteral("udp"));
    formLayout->addRow(tr("Protocol:"), m_protocolCombo);

    m_hostAddressEdit = new QLineEdit(this);
    m_hostAddressEdit->setPlaceholderText(tr("Leave empty for all interfaces"));
    formLayout->addRow(tr("Host Address:"), m_hostAddressEdit);

    m_hostPortSpin = new QSpinBox(this);
    m_hostPortSpin->setRange(1, 65535);
    m_hostPortSpin->setValue(2222);
    formLayout->addRow(tr("Host Port:"), m_hostPortSpin);

    m_guestAddressEdit = new QLineEdit(this);
    m_guestAddressEdit->setPlaceholderText(tr("Leave empty for guest default"));
    formLayout->addRow(tr("Guest Address:"), m_guestAddressEdit);

    m_guestPortSpin = new QSpinBox(this);
    m_guestPortSpin->setRange(1, 65535);
    m_guestPortSpin->setValue(22);
    formLayout->addRow(tr("Guest Port:"), m_guestPortSpin);

    mainLayout->addLayout(formLayout);

    QLabel *infoLabel = new QLabel(this);
    infoLabel->setText(tr("<small>Example: Host port 2222 → Guest port 22 allows SSH access<br>"
                         "via <tt>ssh -p 2222 user@localhost</tt></small>"));
    infoLabel->setStyleSheet(QStringLiteral("color: gray;"));
    mainLayout->addWidget(infoLabel);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_okBtn = buttonBox->button(QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    connect(m_hostPortSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &PortForwardDialog::validate);
    connect(m_guestPortSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &PortForwardDialog::validate);
}

void PortForwardDialog::validate()
{
    m_okBtn->setEnabled(m_hostPortSpin->value() > 0 && m_guestPortSpin->value() > 0);
}

PortForward PortForwardDialog::getPortForward() const
{
    PortForward forward;
    forward.protocol = m_protocolCombo->currentText();
    forward.hostPort = m_hostPortSpin->value();
    forward.guestPort = m_guestPortSpin->value();
    forward.hostAddress = m_hostAddressEdit->text().trimmed();
    forward.guestAddress = m_guestAddressEdit->text().trimmed();
    return forward;
}

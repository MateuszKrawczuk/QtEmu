// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "createbridgedialog.h"
#include "../utils/networkutils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QProcess>
#include <QApplication>

CreateBridgeDialog::CreateBridgeDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadInterfaces();
}

CreateBridgeDialog::~CreateBridgeDialog()
{
}

void CreateBridgeDialog::setupUI()
{
    setWindowTitle(tr("Create Network Bridge"));
    setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    m_bridgeNameEdit = new QLineEdit(this);
    m_bridgeNameEdit->setPlaceholderText(tr("e.g., br0, br1"));
    formLayout->addRow(tr("Bridge name:"), m_bridgeNameEdit);

    QHBoxLayout *interfaceLayout = new QHBoxLayout();
    m_interfaceCombo = new QComboBox(this);
    m_interfaceCombo->addItem(tr("(none - bridge only)"), QString());
    m_refreshBtn = new QPushButton(tr("Refresh"), this);
    connect(m_refreshBtn, &QPushButton::clicked, this, &CreateBridgeDialog::onRefreshInterfacesClicked);
    interfaceLayout->addWidget(m_interfaceCombo);
    interfaceLayout->addWidget(m_refreshBtn);
    formLayout->addRow(tr("Physical interface:"), interfaceLayout);

    mainLayout->addLayout(formLayout);

    m_infoLabel = new QLabel(this);
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setText(tr("<b>Note:</b> Creating a bridge requires administrator privileges.<br>"
                           "You will be prompted for your password via pkexec.<br><br>"
                           "The bridge will also be added to QEMU's bridge configuration<br>"
                           "at /etc/qemu/bridge.conf."));
    m_infoLabel->setStyleSheet(QStringLiteral("color: gray;"));
    mainLayout->addWidget(m_infoLabel);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->hide();
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addStretch();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    m_createBtn = buttonBox->addButton(tr("Create Bridge"), QDialogButtonBox::AcceptRole);
    m_cancelBtn = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(m_createBtn, &QPushButton::clicked, this, &CreateBridgeDialog::onCreateClicked);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    if (!NetworkUtils::canUsePkexec()) {
        m_createBtn->setEnabled(false);
        m_statusLabel->setText(tr("<b>Error:</b> pkexec is not available. Install polkit."));
        m_statusLabel->setStyleSheet(QStringLiteral("color: red;"));
        m_statusLabel->show();
    }
}

void CreateBridgeDialog::loadInterfaces()
{
    m_interfaceCombo->clear();
    m_interfaceCombo->addItem(tr("(none - bridge only)"), QString());
    
    QStringList interfaces = NetworkUtils::getPhysicalInterfaces();
    for (const QString &iface : interfaces) {
        m_interfaceCombo->addItem(iface, iface);
    }
}

void CreateBridgeDialog::onRefreshInterfacesClicked()
{
    loadInterfaces();
}

void CreateBridgeDialog::onCreateClicked()
{
    QString name = m_bridgeNameEdit->text().trimmed();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Name"), 
                           tr("Please enter a name for the bridge."));
        return;
    }
    
    if (NetworkUtils::bridgeExists(name)) {
        QMessageBox::warning(this, tr("Bridge Exists"), 
                           tr("A bridge named '%1' already exists.").arg(name));
        return;
    }

    QString iface = m_interfaceCombo->currentData().toString();
    QString errorMsg;
    
    m_statusLabel->setText(tr("Creating bridge... Please enter your password if prompted."));
    m_statusLabel->setStyleSheet(QStringLiteral("color: blue;"));
    m_statusLabel->show();
    QApplication::processEvents();

    if (NetworkUtils::createBridge(name, iface, errorMsg)) {
        m_statusLabel->setText(tr("Bridge '%1' created successfully!").arg(name));
        m_statusLabel->setStyleSheet(QStringLiteral("color: green;"));
        QMessageBox::information(this, tr("Bridge Created"), 
                               tr("Network bridge '%1' has been created successfully.\n\n"
                                  "You can now use it in your VM network configuration.").arg(name));
        accept();
    } else {
        m_statusLabel->setText(tr("Failed to create bridge: %1").arg(errorMsg));
        m_statusLabel->setStyleSheet(QStringLiteral("color: red;"));
        QMessageBox::critical(this, tr("Bridge Creation Failed"), 
                            tr("Could not create bridge '%1':\n\n%2").arg(name, errorMsg));
    }
}

QString CreateBridgeDialog::bridgeName() const
{
    return m_bridgeNameEdit->text().trimmed();
}

QString CreateBridgeDialog::physicalInterface() const
{
    return m_interfaceCombo->currentData().toString();
}

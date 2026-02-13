// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "networkadapterdialog.h"
#include "createbridgedialog.h"
#include "../utils/networkutils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QRegularExpressionValidator>

NetworkAdapterDialog::NetworkAdapterDialog(NetworkAdapter *adapter,
                                           bool enableFields,
                                           QWidget *parent)
    : QDialog(parent)
    , m_adapter(adapter)
    , m_enableFields(enableFields)
{
    setupUI();
    loadFromAdapter();
}

NetworkAdapterDialog::~NetworkAdapterDialog()
{
}

void NetworkAdapterDialog::setupUI()
{
    setWindowTitle(tr("Network Adapter Configuration"));
    setMinimumWidth(450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    m_idEdit = new QLineEdit(this);
    m_idEdit->setEnabled(m_enableFields);
    formLayout->addRow(tr("Adapter ID:"), m_idEdit);

    m_backendCombo = new QComboBox(this);
    m_backendCombo->addItem(tr("None (disabled)"), static_cast<int>(NetworkBackend::None));
    m_backendCombo->addItem(tr("User Mode (NAT)"), static_cast<int>(NetworkBackend::User));
    m_backendCombo->addItem(tr("Bridge"), static_cast<int>(NetworkBackend::Bridge));
    m_backendCombo->addItem(tr("TAP"), static_cast<int>(NetworkBackend::Tap));
    m_backendCombo->addItem(tr("Socket (VM-to-VM)"), static_cast<int>(NetworkBackend::Socket));
    m_backendCombo->setEnabled(m_enableFields);
    connect(m_backendCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NetworkAdapterDialog::onBackendChanged);
    formLayout->addRow(tr("Backend Type:"), m_backendCombo);

    mainLayout->addLayout(formLayout);

    m_bridgeGroup = new QGroupBox(tr("Bridge Options"), this);
    QHBoxLayout *bridgeLayout = new QHBoxLayout(m_bridgeGroup);
    m_bridgeCombo = new QComboBox(m_bridgeGroup);
    m_bridgeCombo->setEnabled(m_enableFields);
    m_refreshBridgeBtn = new QPushButton(tr("Refresh"), m_bridgeGroup);
    m_refreshBridgeBtn->setEnabled(m_enableFields);
    connect(m_refreshBridgeBtn, &QPushButton::clicked, this, [this]() {
        m_bridgeCombo->clear();
        m_bridgeCombo->addItems(NetworkUtils::getAvailableBridges());
    });
    m_createBridgeBtn = new QPushButton(tr("Create Bridge..."), m_bridgeGroup);
    m_createBridgeBtn->setEnabled(m_enableFields);
    connect(m_createBridgeBtn, &QPushButton::clicked, this, [this]() {
        CreateBridgeDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            m_bridgeCombo->clear();
            m_bridgeCombo->addItems(NetworkUtils::getAvailableBridges());
        }
    });
    bridgeLayout->addWidget(m_bridgeCombo);
    bridgeLayout->addWidget(m_refreshBridgeBtn);
    bridgeLayout->addWidget(m_createBridgeBtn);
    mainLayout->addWidget(m_bridgeGroup);

    m_tapGroup = new QGroupBox(tr("TAP Options"), this);
    QFormLayout *tapLayout = new QFormLayout(m_tapGroup);
    m_tapDeviceEdit = new QLineEdit(m_tapGroup);
    m_tapDeviceEdit->setEnabled(m_enableFields);
    m_tapDeviceEdit->setPlaceholderText(tr("Leave empty for auto-creation"));
    tapLayout->addRow(tr("TAP Device:"), m_tapDeviceEdit);
    mainLayout->addWidget(m_tapGroup);

    m_socketGroup = new QGroupBox(tr("Socket Options (VM-to-VM)"), this);
    QFormLayout *socketLayout = new QFormLayout(m_socketGroup);
    m_socketListenEdit = new QLineEdit(m_socketGroup);
    m_socketListenEdit->setEnabled(m_enableFields);
    m_socketListenEdit->setPlaceholderText(tr("e.g., :1234 or 127.0.0.1:1234"));
    socketLayout->addRow(tr("Listen on:"), m_socketListenEdit);
    m_socketConnectEdit = new QLineEdit(m_socketGroup);
    m_socketConnectEdit->setEnabled(m_enableFields);
    m_socketConnectEdit->setPlaceholderText(tr("e.g., 127.0.0.1:1234"));
    socketLayout->addRow(tr("Connect to:"), m_socketConnectEdit);
    mainLayout->addWidget(m_socketGroup);

    QFormLayout *deviceForm = new QFormLayout();

    m_nicModelCombo = new QComboBox(this);
    m_nicModelCombo->addItem(tr("Virtio (best performance)"), static_cast<int>(NicModel::Virtio));
    m_nicModelCombo->addItem(tr("Intel E1000 (compatible)"), static_cast<int>(NicModel::E1000));
    m_nicModelCombo->addItem(tr("Intel E1000E (newer)"), static_cast<int>(NicModel::E1000E));
    m_nicModelCombo->addItem(tr("Realtek RTL8139"), static_cast<int>(NicModel::Rtl8139));
    m_nicModelCombo->addItem(tr("AMD PCnet"), static_cast<int>(NicModel::Pcnet));
    m_nicModelCombo->addItem(tr("Intel 82551"), static_cast<int>(NicModel::I82551));
    m_nicModelCombo->addItem(tr("Intel 82557B"), static_cast<int>(NicModel::I82557B));
    m_nicModelCombo->addItem(tr("Intel 82559ER"), static_cast<int>(NicModel::I82559er));
    m_nicModelCombo->setEnabled(m_enableFields);
    deviceForm->addRow(tr("NIC Model:"), m_nicModelCombo);

    QHBoxLayout *macLayout = new QHBoxLayout();
    m_macEdit = new QLineEdit(this);
    m_macEdit->setEnabled(m_enableFields);
    m_macEdit->setPlaceholderText(tr("Auto-generated if empty"));
    QRegularExpression macRegex(QStringLiteral("([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}"));
    m_macEdit->setValidator(new QRegularExpressionValidator(macRegex, this));
    m_generateMacBtn = new QPushButton(tr("Generate"), this);
    m_generateMacBtn->setEnabled(m_enableFields);
    connect(m_generateMacBtn, &QPushButton::clicked, this, &NetworkAdapterDialog::onGenerateMacClicked);
    m_autoMacCheck = new QCheckBox(tr("Auto"), this);
    m_autoMacCheck->setEnabled(m_enableFields);
    connect(m_autoMacCheck, &QCheckBox::clicked, this, &NetworkAdapterDialog::onAutoMacClicked);
    macLayout->addWidget(m_macEdit);
    macLayout->addWidget(m_generateMacBtn);
    macLayout->addWidget(m_autoMacCheck);
    deviceForm->addRow(tr("MAC Address:"), macLayout);

    m_bootRomCheck = new QCheckBox(tr("Enable boot from network (PXE)"), this);
    m_bootRomCheck->setEnabled(m_enableFields);
    deviceForm->addRow(QString(), m_bootRomCheck);

    mainLayout->addLayout(deviceForm);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_okBtn = buttonBox->button(QDialogButtonBox::Ok);
    m_okBtn->setEnabled(m_enableFields);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    m_bridgeCombo->addItems(NetworkUtils::getAvailableBridges());
    updateBackendOptions();
}

void NetworkAdapterDialog::loadFromAdapter()
{
    if (!m_adapter) return;

    m_idEdit->setText(m_adapter->id());

    int backendIndex = m_backendCombo->findData(static_cast<int>(m_adapter->backend()));
    if (backendIndex >= 0) {
        m_backendCombo->setCurrentIndex(backendIndex);
    }

    int nicIndex = m_nicModelCombo->findData(static_cast<int>(m_adapter->nicModel()));
    if (nicIndex >= 0) {
        m_nicModelCombo->setCurrentIndex(nicIndex);
    }

    m_macEdit->setText(m_adapter->macAddress());
    m_bridgeCombo->setCurrentText(m_adapter->bridgeName());
    m_tapDeviceEdit->setText(m_adapter->tapDevice());
    m_socketListenEdit->setText(m_adapter->socketListen());
    m_socketConnectEdit->setText(m_adapter->socketConnect());
    m_bootRomCheck->setChecked(m_adapter->bootROM());

    updateBackendOptions();
}

void NetworkAdapterDialog::saveToAdapter()
{
    if (!m_adapter) return;

    m_adapter->setId(m_idEdit->text());

    int backendValue = m_backendCombo->currentData().toInt();
    m_adapter->setBackend(static_cast<NetworkBackend>(backendValue));

    int nicValue = m_nicModelCombo->currentData().toInt();
    m_adapter->setNicModel(static_cast<NicModel>(nicValue));

    m_adapter->setMacAddress(m_macEdit->text().trimmed());
    m_adapter->setBridgeName(m_bridgeCombo->currentText());
    m_adapter->setTapDevice(m_tapDeviceEdit->text().trimmed());
    m_adapter->setSocketListen(m_socketListenEdit->text().trimmed());
    m_adapter->setSocketConnect(m_socketConnectEdit->text().trimmed());
    m_adapter->setBootROM(m_bootRomCheck->isChecked());
}

void NetworkAdapterDialog::onBackendChanged(int index)
{
    Q_UNUSED(index);
    updateBackendOptions();
}

void NetworkAdapterDialog::onGenerateMacClicked()
{
    m_macEdit->setText(NetworkUtils::generateMacAddress());
    m_autoMacCheck->setChecked(false);
}

void NetworkAdapterDialog::onAutoMacClicked()
{
    if (m_autoMacCheck->isChecked()) {
        m_macEdit->clear();
    }
}

void NetworkAdapterDialog::updateBackendOptions()
{
    int backendValue = m_backendCombo->currentData().toInt();
    NetworkBackend backend = static_cast<NetworkBackend>(backendValue);

    m_bridgeGroup->setVisible(backend == NetworkBackend::Bridge);
    m_tapGroup->setVisible(backend == NetworkBackend::Tap);
    m_socketGroup->setVisible(backend == NetworkBackend::Socket);

    bool isNone = (backend == NetworkBackend::None);
    m_nicModelCombo->setEnabled(!isNone && m_enableFields);
    m_macEdit->setEnabled(!isNone && m_enableFields);
    m_generateMacBtn->setEnabled(!isNone && m_enableFields);
    m_autoMacCheck->setEnabled(!isNone && m_enableFields);
    m_bootRomCheck->setEnabled(!isNone && m_enableFields);
}

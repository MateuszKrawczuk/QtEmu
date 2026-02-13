// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "machineconfignetwork.h"
#include "networkadapterdialog.h"
#include "createbridgedialog.h"
#include "../networkadapter.h"
#include "../utils/networkutils.h"
#include <QHeaderView>
#include <QMessageBox>

MachineConfigNetwork::MachineConfigNetwork(Machine *machine,
                                           QWidget *parent) : QWidget(parent)
{
    m_enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        m_enableFields = false;
    }

    m_machine = machine;
    setupUI();
    loadAdapters();
}

MachineConfigNetwork::~MachineConfigNetwork()
{
}

void MachineConfigNetwork::setupUI()
{
    m_networkLayout = new QVBoxLayout();
    m_networkLayout->setAlignment(Qt::AlignTop);
    m_networkLayout->setContentsMargins(5, 20, 5, 0);

    QLabel *titleLabel = new QLabel(tr("Network Adapters"));
    titleLabel->setStyleSheet(QStringLiteral("font-weight: bold;"));
    m_networkLayout->addWidget(titleLabel);

    m_adapterTable = new QTableWidget(this);
    m_adapterTable->setColumnCount(4);
    m_adapterTable->setHorizontalHeaderLabels({
        tr("ID"),
        tr("Backend"),
        tr("NIC Model"),
        tr("MAC Address")
    });
    m_adapterTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_adapterTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_adapterTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_adapterTable->horizontalHeader()->setStretchLastSection(true);
    m_adapterTable->verticalHeader()->setVisible(false);
    connect(m_adapterTable, &QTableWidget::itemSelectionChanged,
            this, &MachineConfigNetwork::onAdapterSelectionChanged);
    connect(m_adapterTable, &QTableWidget::cellDoubleClicked,
            this, &MachineConfigNetwork::onEditAdapterClicked);
    m_networkLayout->addWidget(m_adapterTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addAdapterBtn = new QPushButton(tr("Add Adapter"), this);
    m_addAdapterBtn->setEnabled(m_enableFields);
    connect(m_addAdapterBtn, &QPushButton::clicked, this, &MachineConfigNetwork::onAddAdapterClicked);
    
    m_editAdapterBtn = new QPushButton(tr("Edit"), this);
    m_editAdapterBtn->setEnabled(false);
    connect(m_editAdapterBtn, &QPushButton::clicked, this, &MachineConfigNetwork::onEditAdapterClicked);
    
    m_removeAdapterBtn = new QPushButton(tr("Remove"), this);
    m_removeAdapterBtn->setEnabled(false);
    connect(m_removeAdapterBtn, &QPushButton::clicked, this, &MachineConfigNetwork::onRemoveAdapterClicked);
    
    buttonLayout->addWidget(m_addAdapterBtn);
    buttonLayout->addWidget(m_editAdapterBtn);
    buttonLayout->addWidget(m_removeAdapterBtn);
    buttonLayout->addStretch();
    m_networkLayout->addLayout(buttonLayout);

    m_infoLabel = new QLabel(this);
    m_infoLabel->setWordWrap(true);
    
    QString infoText;
    if (NetworkUtils::isBridgeSupported()) {
        infoText = tr("<b>Available network backends:</b><br>"
                     "• <b>User Mode (NAT):</b> Basic connectivity, no setup required<br>"
                     "• <b>Bridge:</b> VM appears as separate host on network (requires setup)<br>"
                     "• <b>TAP:</b> Advanced networking, requires root access<br>"
                     "• <b>Socket:</b> Direct VM-to-VM communication<br><br>"
                     "<b>Tip:</b> Use Virtio NIC for best performance on Linux guests.");
    } else {
        infoText = tr("<b>Available network backends:</b><br>"
                     "• <b>User Mode (NAT):</b> Basic connectivity, no setup required<br>"
                     "• <b>Socket:</b> Direct VM-to-VM communication<br><br>"
                     "<b>Note:</b> Bridge networking requires Linux with proper configuration.");
    }
    m_infoLabel->setText(infoText);
    m_infoLabel->setStyleSheet(QStringLiteral("color: gray; font-size: 10px;"));
    m_networkLayout->addWidget(m_infoLabel);

    m_machineNetworkGroup = new QGroupBox(tr("Machine Network"));
    m_machineNetworkGroup->setLayout(m_networkLayout);

    m_networkMainLayout = new QVBoxLayout();
    m_networkMainLayout->addWidget(m_machineNetworkGroup);

    m_networkPageWidget = new QWidget();
    m_networkPageWidget->setLayout(m_networkMainLayout);

    qDebug() << "MachineConfigNetwork created";
}

void MachineConfigNetwork::loadAdapters()
{
    updateTable();
}

void MachineConfigNetwork::updateTable()
{
    m_adapterTable->setRowCount(0);
    
    QList<NetworkAdapter*> adapters = m_machine->getNetworkAdapters();
    
    for (int i = 0; i < adapters.size(); ++i) {
        NetworkAdapter *adapter = adapters.at(i);
        int row = m_adapterTable->rowCount();
        m_adapterTable->insertRow(row);
        
        m_adapterTable->setItem(row, 0, new QTableWidgetItem(adapter->id()));
        m_adapterTable->setItem(row, 1, new QTableWidgetItem(backendToString(adapter->backend())));
        m_adapterTable->setItem(row, 2, new QTableWidgetItem(nicModelToString(adapter->nicModel())));
        m_adapterTable->setItem(row, 3, new QTableWidgetItem(
            adapter->macAddress().isEmpty() ? tr("(auto)") : adapter->macAddress()));
    }
    
    m_adapterTable->resizeColumnsToContents();
}

QString MachineConfigNetwork::backendToString(NetworkBackend backend)
{
    switch (backend) {
    case NetworkBackend::None: return tr("None");
    case NetworkBackend::User: return tr("User Mode (NAT)");
    case NetworkBackend::Bridge: return tr("Bridge");
    case NetworkBackend::Tap: return tr("TAP");
    case NetworkBackend::Socket: return tr("Socket");
    default: return tr("Unknown");
    }
}

QString MachineConfigNetwork::nicModelToString(NicModel model)
{
    switch (model) {
    case NicModel::Virtio: return QStringLiteral("Virtio");
    case NicModel::E1000: return QStringLiteral("E1000");
    case NicModel::E1000E: return QStringLiteral("E1000E");
    case NicModel::Rtl8139: return QStringLiteral("RTL8139");
    case NicModel::Pcnet: return QStringLiteral("PCnet");
    case NicModel::I82551: return QStringLiteral("i82551");
    case NicModel::I82557B: return QStringLiteral("i82557b");
    case NicModel::I82559er: return QStringLiteral("i82559er");
    default: return tr("Unknown");
    }
}

void MachineConfigNetwork::onAddAdapterClicked()
{
    QString newId = QStringLiteral("net%1").arg(m_machine->networkAdapterCount());
    
    NetworkAdapter *adapter = new NetworkAdapter(m_machine);
    adapter->setId(newId);
    adapter->setBackend(NetworkBackend::User);
    adapter->setNicModel(NicModel::Virtio);
    
    NetworkAdapterDialog dialog(adapter, m_enableFields, this);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToAdapter();
        m_machine->addNetworkAdapter(adapter);
        updateTable();
    } else {
        delete adapter;
    }
}

void MachineConfigNetwork::onEditAdapterClicked()
{
    int row = m_adapterTable->currentRow();
    if (row < 0 || row >= m_machine->networkAdapterCount()) return;
    
    NetworkAdapter *adapter = m_machine->getNetworkAdapter(row);
    if (!adapter) return;
    
    NetworkAdapterDialog dialog(adapter, m_enableFields, this);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToAdapter();
        updateTable();
    }
}

void MachineConfigNetwork::onRemoveAdapterClicked()
{
    int row = m_adapterTable->currentRow();
    if (row < 0 || row >= m_machine->networkAdapterCount()) return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Remove Adapter"),
        tr("Are you sure you want to remove this network adapter?"),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        m_machine->removeNetworkAdapter(row);
        updateTable();
    }
}

void MachineConfigNetwork::onAdapterSelectionChanged()
{
    bool hasSelection = m_adapterTable->currentRow() >= 0;
    m_editAdapterBtn->setEnabled(hasSelection && m_enableFields);
    m_removeAdapterBtn->setEnabled(hasSelection && m_enableFields);
}

void MachineConfigNetwork::saveNetworkData()
{
    // Data is already saved to NetworkAdapter objects in the machine
    // This method is kept for compatibility with the config window API
}

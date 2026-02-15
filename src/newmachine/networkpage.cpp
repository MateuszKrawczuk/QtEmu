// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "networkpage.h"
#include "../machineconfig/networkadapterdialog.h"
#include "../networkadapter.h"
#include "../utils/networkutils.h"
#include <QHeaderView>
#include <QtGlobal>

NetworkPage::NetworkPage(Machine *machine, QWidget *parent)
    : QWizardPage(parent), m_machine(machine), m_adaptersLoaded(false)
{
    setTitle(tr("Network Configuration"));
    setSubTitle(tr("Configure network adapters for the virtual machine."));
    
    setupUI();
}

NetworkPage::~NetworkPage()
{
}

void NetworkPage::setupUI()
{
    m_mainLayout = new QVBoxLayout();
    
    m_enableNetworkCheck = new QCheckBox(tr("Enable network connectivity"), this);
    m_enableNetworkCheck->setChecked(true);
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    connect(m_enableNetworkCheck, &QCheckBox::checkStateChanged,
            this, [this](Qt::CheckState state) { onEnableNetworkChanged(static_cast<int>(state)); });
#else
    connect(m_enableNetworkCheck, QOverload<int>::of(&QCheckBox::stateChanged),
            this, &NetworkPage::onEnableNetworkChanged);
#endif
    m_mainLayout->addWidget(m_enableNetworkCheck);
    
    m_adaptersGroup = new QGroupBox(tr("Network Adapters"), this);
    QVBoxLayout *adaptersLayout = new QVBoxLayout();
    
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
    m_adapterTable->setMinimumHeight(120);
    connect(m_adapterTable, &QTableWidget::itemSelectionChanged,
            this, &NetworkPage::onAdapterSelectionChanged);
    connect(m_adapterTable, &QTableWidget::cellDoubleClicked,
            this, &NetworkPage::onEditAdapterClicked);
    adaptersLayout->addWidget(m_adapterTable);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addAdapterBtn = new QPushButton(tr("Add"), this);
    m_editAdapterBtn = new QPushButton(tr("Edit"), this);
    m_removeAdapterBtn = new QPushButton(tr("Remove"), this);
    m_editAdapterBtn->setEnabled(false);
    m_removeAdapterBtn->setEnabled(false);
    
    connect(m_addAdapterBtn, &QPushButton::clicked, this, &NetworkPage::onAddAdapterClicked);
    connect(m_editAdapterBtn, &QPushButton::clicked, this, &NetworkPage::onEditAdapterClicked);
    connect(m_removeAdapterBtn, &QPushButton::clicked, this, &NetworkPage::onRemoveAdapterClicked);
    
    buttonLayout->addWidget(m_addAdapterBtn);
    buttonLayout->addWidget(m_editAdapterBtn);
    buttonLayout->addWidget(m_removeAdapterBtn);
    buttonLayout->addStretch();
    adaptersLayout->addLayout(buttonLayout);
    
    m_adaptersGroup->setLayout(adaptersLayout);
    m_mainLayout->addWidget(m_adaptersGroup);
    
    m_infoLabel = new QLabel(this);
    m_infoLabel->setWordWrap(true);
    QString infoText;
    if (NetworkUtils::isBridgeSupported()) {
        infoText = tr("<b>Available backends:</b> User Mode (NAT), Bridge, TAP, Socket<br>"
                     "<b>Tip:</b> Use Virtio for best performance. For external network "
                     "access, Bridge mode requires setup.");
    } else {
        infoText = tr("<b>Available backends:</b> User Mode (NAT), Socket<br>"
                     "<b>Note:</b> Bridge/TAP require Linux with proper configuration.");
    }
    m_infoLabel->setText(infoText);
    m_infoLabel->setStyleSheet(QStringLiteral("color: gray; font-size: 10px;"));
    m_mainLayout->addWidget(m_infoLabel);
    
    m_mainLayout->addStretch();
    setLayout(m_mainLayout);
    
    registerField("enableNetwork", m_enableNetworkCheck);
}

void NetworkPage::initializePage()
{
    if (!m_adaptersLoaded) {
        if (m_machine->networkAdapterCount() == 0) {
            addDefaultAdapter();
        }
        m_adaptersLoaded = true;
    }
    updateTable();
    onEnableNetworkChanged(m_enableNetworkCheck->checkState());
}

void NetworkPage::addDefaultAdapter()
{
    NetworkAdapter *adapter = new NetworkAdapter(m_machine);
    adapter->setId(QStringLiteral("net0"));
    adapter->setBackend(NetworkBackend::User);
    adapter->setNicModel(NicModel::Virtio);
    m_machine->addNetworkAdapter(adapter);
}

void NetworkPage::updateTable()
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

QString NetworkPage::backendToString(NetworkBackend backend)
{
    switch (backend) {
    case NetworkBackend::None: return tr("None");
    case NetworkBackend::User: return tr("User (NAT)");
    case NetworkBackend::Bridge: return tr("Bridge");
    case NetworkBackend::Tap: return tr("TAP");
    case NetworkBackend::Socket: return tr("Socket");
    default: return tr("Unknown");
    }
}

QString NetworkPage::nicModelToString(NicModel model)
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

void NetworkPage::onEnableNetworkChanged(int state)
{
    bool enabled = (state == Qt::Checked);
    m_adaptersGroup->setEnabled(enabled);
    m_adapterTable->setEnabled(enabled);
    m_addAdapterBtn->setEnabled(enabled);
    m_editAdapterBtn->setEnabled(enabled && m_adapterTable->currentRow() >= 0);
    m_removeAdapterBtn->setEnabled(enabled && m_adapterTable->currentRow() >= 0);
}

void NetworkPage::onAddAdapterClicked()
{
    int nextNum = m_machine->networkAdapterCount();
    QString newId = QStringLiteral("net%1").arg(nextNum);
    
    while ([this, &newId]() {
        for (int i = 0; i < m_machine->networkAdapterCount(); ++i) {
            if (m_machine->getNetworkAdapter(i)->id() == newId) {
                return true;
            }
        }
        return false;
    }()) {
        newId = QStringLiteral("net%1").arg(++nextNum);
    }
    
    NetworkAdapter *adapter = new NetworkAdapter(m_machine);
    adapter->setId(newId);
    adapter->setBackend(NetworkBackend::User);
    adapter->setNicModel(NicModel::Virtio);
    
    NetworkAdapterDialog dialog(adapter, true, this);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToAdapter();
        m_machine->addNetworkAdapter(adapter);
        updateTable();
    } else {
        delete adapter;
    }
}

void NetworkPage::onEditAdapterClicked()
{
    int row = m_adapterTable->currentRow();
    if (row < 0 || row >= m_machine->networkAdapterCount()) return;
    
    NetworkAdapter *adapter = m_machine->getNetworkAdapter(row);
    if (!adapter) return;
    
    NetworkAdapterDialog dialog(adapter, true, this);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToAdapter();
        updateTable();
    }
}

void NetworkPage::onRemoveAdapterClicked()
{
    int row = m_adapterTable->currentRow();
    if (row < 0 || row >= m_machine->networkAdapterCount()) return;
    
    m_machine->removeNetworkAdapter(row);
    updateTable();
    onAdapterSelectionChanged();
}

void NetworkPage::onAdapterSelectionChanged()
{
    bool hasSelection = m_adapterTable->currentRow() >= 0;
    m_editAdapterBtn->setEnabled(hasSelection && m_enableNetworkCheck->isChecked());
    m_removeAdapterBtn->setEnabled(hasSelection && m_enableNetworkCheck->isChecked());
}

bool NetworkPage::validatePage()
{
    bool enableNetwork = m_enableNetworkCheck->isChecked();
    m_machine->setUseNetwork(enableNetwork);
    
    if (!enableNetwork) {
        while (m_machine->networkAdapterCount() > 0) {
            m_machine->removeNetworkAdapter(0);
        }
    }
    
    return true;
}

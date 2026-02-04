// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "machineconfigaccel.h"

#include <QRadioButton>
#include <QProcess>

/**
 * @brief Accelerator configuration window
 * @param machine, machine to be configured
 * @param qemuObject, QEMU object with data about QEMU
 * @param parent, parent widget
 *
 * In this window the user can select what accelerators want to use and
 * it's order
 */
MachineConfigAccel::MachineConfigAccel(Machine *machine,
                                       QEMU *qemuObject,
                                       QWidget *parent) : QWidget(parent)
{
    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    this->m_machine = machine;
    this->m_qemuObject = qemuObject;

    m_acceleratorGroup = new QButtonGroup(this);

    m_tcgRadio = new QRadioButton(tr("TCG (default)"), this);
    m_acceleratorGroup->addButton(m_tcgRadio);

    #ifdef Q_OS_LINUX
    m_kvmRadio = new QRadioButton(tr("KVM"), this);
    m_xenRadio = new QRadioButton(tr("XEN"), this);
    m_acceleratorGroup->addButton(m_kvmRadio);
    m_acceleratorGroup->addButton(m_xenRadio);
    #endif

    #ifdef Q_OS_WIN
    m_whpxRadio = new QRadioButton(tr("WHPX"), this);
    m_haxmRadio = new QRadioButton(tr("HAXM"), this);
    m_acceleratorGroup->addButton(m_whpxRadio);
    m_acceleratorGroup->addButton(m_haxmRadio);

    // Check if QEMU version is > 8.0 (HAXM removed in QEMU 8.1+)
    bool isQemuNewerThan8 = m_qemuObject->isQEMUVersionGreaterThan(8, 0);

    if (isQemuNewerThan8) {
        m_haxmRadio->setEnabled(false);
        m_haxmRadio->setToolTip(tr("HAXM is not supported in QEMU version > 8.0"));
    }

    // Check if HAXM driver is installed and running
    bool haxmInstalled = false;
    QProcess process;
    process.start("sc", QStringList() << "query" << "IntelHaxm");
    process.waitForFinished(3000);
    QString output = process.readAllStandardOutput();
    haxmInstalled = output.contains("RUNNING");

    if (!haxmInstalled && !isQemuNewerThan8) {
        m_haxmRadio->setToolTip(tr("HAXM driver is not installed or not running"));
    }

    // Check if Windows Hypervisor Platform is enabled
    bool whpxEnabled = false;
    process.start("powershell", QStringList() << "-Command" <<
                 "Get-WindowsOptionalFeature -Online -FeatureName HypervisorPlatform | Select-Object -ExpandProperty State");
    process.waitForFinished(3000);
    output = process.readAllStandardOutput();
    whpxEnabled = output.contains("Enabled");

    if (!whpxEnabled) {
        m_whpxRadio->setToolTip(tr("Windows Hypervisor Platform is not enabled"));
    }
    #endif

    #ifdef Q_OS_MACOS
    m_hvfRadio = new QRadioButton(tr("HVF"), this);
    m_acceleratorGroup->addButton(m_hvfRadio);
    #endif

    QString currentAccel = machine->getAccelerator().isEmpty() ?
                          "tcg" : machine->getAccelerator().first();

    // Default to TCG
    m_tcgRadio->setChecked(true);

    // Then check platform-specific options
    #ifdef Q_OS_LINUX
    if (currentAccel == "kvm" && m_kvmRadio != nullptr)
        m_kvmRadio->setChecked(true);
    else if (currentAccel == "xen" && m_xenRadio != nullptr)
        m_xenRadio->setChecked(true);
    #endif
    #ifdef Q_OS_WIN
    if (currentAccel == "whpx" && m_whpxRadio != nullptr)
        m_whpxRadio->setChecked(true);
    else if (currentAccel == "hax" && m_haxmRadio != nullptr && m_haxmRadio->isEnabled())
        m_haxmRadio->setChecked(true);
    #endif
    #ifdef Q_OS_MACOS
    if (currentAccel == "hvf" && m_hvfRadio != nullptr)
        m_hvfRadio->setChecked(true);
    #endif

    QVBoxLayout *radioLayout = new QVBoxLayout();
    radioLayout->addWidget(m_tcgRadio);
    #ifdef Q_OS_LINUX
    radioLayout->addWidget(m_kvmRadio);
    radioLayout->addWidget(m_xenRadio);
    #endif
    #ifdef Q_OS_WIN
    radioLayout->addWidget(m_whpxRadio);
    radioLayout->addWidget(m_haxmRadio);
    #endif
    #ifdef Q_OS_MACOS
    radioLayout->addWidget(m_hvfRadio);
    #endif

    m_acceleratorLayout = new QVBoxLayout();
    m_acceleratorLayout->setAlignment(Qt::AlignTop);
    m_acceleratorLayout->addLayout(radioLayout);

    m_acceleratorPageWidget = new QWidget();
    m_acceleratorPageWidget->setLayout(m_acceleratorLayout);

    qDebug() << "MachineConfigAccel created";
}

MachineConfigAccel::~MachineConfigAccel()
{
    qDebug() << "MachineConfigAccel destroyed";
}

/**
 * @brief Move up the selected accelerator
 *
 * Move up one position the selected accelerator
 */
void MachineConfigAccel::moveUpButton()
{
    int index = this->m_acceleratorTree->currentIndex().row();
    if(index < 1 || index > this->m_acceleratorTree->topLevelItemCount()) {
        return;
    }

    QTreeWidgetItem *item = this->m_acceleratorTree->takeTopLevelItem(index);
    this->m_acceleratorTree->insertTopLevelItem(index - 1, item);
    this->m_acceleratorTree->setCurrentItem(item);
}

/**
 * @brief Move down the selected accelerator
 *
 * Move down one position the selected accelerator
 */
void MachineConfigAccel::moveDownButton()
{
    int index = this->m_acceleratorTree->currentIndex().row();
    if (index < 0 || index > this->m_acceleratorTree->topLevelItemCount() - 2) {
        return;
    }

    QTreeWidgetItem *item = this->m_acceleratorTree ->takeTopLevelItem(index);
    this->m_acceleratorTree->insertTopLevelItem(index + 1, item);
    this->m_acceleratorTree->setCurrentItem(item);
}

/**
 * @brief Save the accelerators
 *
 * Save the selected accelerators in the specified order
 */
void MachineConfigAccel::saveAccelData()
{
    m_machine->removeAllAccelerators();

    // Default to TCG if no other accelerator is selected
    if (!m_acceleratorGroup->checkedButton() || m_tcgRadio->isChecked()) {
        m_machine->addAccelerator("tcg");
        return;
    }

    #ifdef Q_OS_LINUX
    if (m_kvmRadio && m_kvmRadio->isChecked()) {
        m_machine->addAccelerator("kvm");
        return;
    }
    if (m_xenRadio && m_xenRadio->isChecked()) {
        m_machine->addAccelerator("xen");
        return;
    }
    #endif

    #ifdef Q_OS_WIN
    if (m_whpxRadio && m_whpxRadio->isChecked()) {
        m_machine->addAccelerator("whpx");
        return;
    }
    if (m_haxmRadio && m_haxmRadio->isChecked()) {
        m_machine->addAccelerator("hax");
        return;
    }
    #endif

    #ifdef Q_OS_MACOS
    if (m_hvfRadio && m_hvfRadio->isChecked()) {
        m_machine->addAccelerator("hvf");
        return;
    }
    #endif

    // Fallback to TCG if somehow no valid accelerator was selected
    m_machine->addAccelerator("tcg");
}

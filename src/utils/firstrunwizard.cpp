/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

// Local
#include "firstrunwizard.h"

/**
 * @brief First run wizard
 * @param parent, parent widget
 *
 * First run wizard. This wizard run one time
 * when the user start QtEmu for first time
 * and allows to select the QEMU options
 */
FirstRunWizard::FirstRunWizard(QWidget *parent) : QWizard(parent)
{
    this->setWindowTitle(tr("First Run Wizard"));
    this->setWindowIcon(QIcon::fromTheme("quickwizard",
                                         QIcon(QPixmap(":/images/icons/breeze/32x32/quickwizard.svg"))));
    m_qemuBinariesPage = new QEMUBinariesPage(this);
    this->setPage(Page_QEMUBinaries, m_qemuBinariesPage);
    this->setStartId(Page_QEMUBinaries);

#ifndef Q_OS_MAC
    this->setWizardStyle(ClassicStyle);
#endif
#ifdef Q_OS_MAC
    this->setWizardStyle(MacStyle);
#endif

    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    this->setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));
    this->setMinimumSize(700, 400);

    this->setButtonText(QWizard::CustomButton1, tr("Cancel"));
    connect(this, &QWizard::customButtonClicked,
            this, &FirstRunWizard::cancelButton);

    QList<QWizard::WizardButton> buttonLayout;
    buttonLayout <<QWizard::Stretch << QWizard::FinishButton << QWizard::CustomButton1;
    this->setButtonLayout(buttonLayout);

    m_warningFinishMessageBox = new QMessageBox();
    m_warningFinishMessageBox->setWindowTitle(tr("QEMU - configuration"));
    m_warningFinishMessageBox->setWindowIcon(QIcon::fromTheme("quickwizard",
                                                              QIcon(QPixmap(":/images/icons/breeze/32x32/window-close.svg"))));
    m_warningFinishMessageBox->setIcon(QMessageBox::Warning);
    m_warningFinishMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    m_warningFinishMessageBox->setText("<p>If you don't finish the Wizard QtEmu will not work correctly.</p>"
                                       "<p>You can make the configuration later from the QtEmu menu</p>");

    qDebug() << "FirstRunWizard created";
}

FirstRunWizard::~FirstRunWizard()
{
    qDebug() << "FirstRunWizard destroyed";
}

/**
 * @brief Close event
 *
 * Trigger then the x in the toolbar is pressed
 */
void FirstRunWizard::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.beginGroup("Configuration");
    settings.setValue("firstrunwizard", false);
    settings.endGroup();
    settings.sync();

    if (!this->m_qemuBinariesPage->isWizardComplete()) {
        this->m_warningFinishMessageBox->exec();
    }

    event->accept();
}

/**
 * @brief Hide event
 *
 * Trigger then the ESC key is pressed
 */
void FirstRunWizard::hideEvent(QHideEvent *event)
{
    event->ignore();
}

/**
 * @brief Ignore ESC key
 *
 * Ignore ESC key
 */
void FirstRunWizard::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        event->ignore();
    }
}

/**
 * @brief Trigger when the cancel button is pressed
 *
 * Trigger when the cancel button is pressed.
 */
void FirstRunWizard::cancelButton()
{
    this->close();
}

/**
 * @brief Page when you can select the QEMU options
 * @param parent, parent widget
 *
 * Page when you can select the QEMU options.
 * QEMU binaries path, qemu-img...etc...
 */
QEMUBinariesPage::QEMUBinariesPage(QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("QEMU options"));

    m_descriptionLabel = new QLabel(tr("Select QEMU binaries folder and qemu-img folder"), this);
    m_descriptionLabel->setWordWrap(true);

    m_qemuBinariesPathLabel = new QLabel(tr("QEMU binaries path") + ":", this);
    m_qemuBinariesPathLineEdit = new QLineEdit(this);
    m_qemuBinariesPushButton = new QPushButton(this);
    m_qemuBinariesPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_qemuBinariesPushButton, &QAbstractButton::clicked,
            this, &QEMUBinariesPage::setQemuBinariesPath);

    m_qemuImgPathLabel = new QLabel(tr("QEMU img path") + ":", this);
    m_qemuImgPathLineEdit = new QLineEdit(this);
    m_qemuImgPathPushButton = new QPushButton(this);
    m_qemuImgPathPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_qemuImgPathPushButton, &QAbstractButton::clicked,
            this, &QEMUBinariesPage::setQemuImgPath);

    m_vmsPathLabel = new QLabel(tr("QEMU machines path") + ":", this);
    m_qemuMachinesPathLineEdit = new QLineEdit(this);
    m_qemuMachinesPathPushButton = new QPushButton(this);
    m_qemuMachinesPathPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_qemuMachinesPathPushButton, &QAbstractButton::clicked,
            this, &QEMUBinariesPage::setQemuMachinesPath);

#ifdef Q_OS_WIN
    m_descriptionHostPortLabel = new QLabel(tr("Select the port to connect to QEMU monitor"), this);
    m_descriptionHostPortLabel->setWordWrap(true);

    m_monitorHostnameLabel = new QLabel(tr("Hostname") + ":", this);
    m_monitorHostnameLabel->setWordWrap(true);

    m_monitorHostnameComboBox = new QComboBox(this);
    m_monitorHostnameComboBox->addItem("localhost");
    m_monitorHostnameComboBox->addItem("127.0.0.1");

    m_monitorSocketPathLabel = new QLabel(tr("Port") + ":", this);
    m_monitorSocketSpinBox = new QSpinBox(this);
    m_monitorSocketSpinBox->setMinimum(1);
    m_monitorSocketSpinBox->setMaximum(65535);
    m_monitorSocketSpinBox->setValue(6000);
#endif

    this->registerField("qemuBinaries*", m_qemuBinariesPathLineEdit);
    this->registerField("qemuImg*", m_qemuImgPathLineEdit);
    this->registerField("qemuMachinesPath*", m_qemuMachinesPathLineEdit);

    m_mainLayout = new QGridLayout();
    m_mainLayout->setColumnStretch(1, 10);
    m_mainLayout->addWidget(m_descriptionLabel,           0, 0, 1, 4);
    m_mainLayout->addWidget(m_qemuBinariesPathLabel,      1, 0, 1, 1);
    m_mainLayout->addWidget(m_qemuBinariesPathLineEdit,   1, 1, 1, 2);
    m_mainLayout->addWidget(m_qemuBinariesPushButton,     1, 2, 1, 1);
    m_mainLayout->addWidget(m_qemuImgPathLabel,           2, 0, 1, 1);
    m_mainLayout->addWidget(m_qemuImgPathLineEdit,        2, 1, 1, 2);
    m_mainLayout->addWidget(m_qemuImgPathPushButton,      2, 2, 1, 1);
    m_mainLayout->addWidget(m_vmsPathLabel,               3, 0, 1, 1);
    m_mainLayout->addWidget(m_qemuMachinesPathLineEdit,   3, 1, 1, 2);
    m_mainLayout->addWidget(m_qemuMachinesPathPushButton, 3, 2, 1, 1);
#ifdef Q_OS_WIN
    m_mainLayout->addWidget(m_descriptionHostPortLabel,   4, 0, 1, 4);
    m_mainLayout->addWidget(m_monitorHostnameLabel,       5, 0, 1, 1);
    m_mainLayout->addWidget(m_monitorHostnameComboBox,    5, 1, 1, 1);
    m_mainLayout->addWidget(m_monitorSocketPathLabel,     6, 0, 1, 1);
    m_mainLayout->addWidget(m_monitorSocketSpinBox,       6, 1, 1, 1);
#endif

    this->setLayout(m_mainLayout);

    qDebug() << "QEMUBinariesPage created";
}

QEMUBinariesPage::~QEMUBinariesPage()
{
    qDebug() << "QEMUBinariesPage destroyed";
}

/**
 * @brief Check if all the fields are completed
 * @return true if the wizard is completed
 *
 * Check if all the fields are completed
 */
bool QEMUBinariesPage::isWizardComplete()
{
    bool complete = false;

    if (!this->m_qemuBinariesPathLineEdit->text().isEmpty() &&
        !this->m_qemuImgPathLineEdit->text().isEmpty() &&
        !this->m_qemuMachinesPathLineEdit->text().isEmpty()) {
        complete = true;
    }

    return complete;
}

/**
 * @brief Set the location of all the QEMU binaries
 *
 * Set the location of all the QEMU binaries
 */
void QEMUBinariesPage::setQemuBinariesPath()
{
    QString qemuBinariesPath = QFileDialog::getExistingDirectory(this, tr("Select QEMU binaries path"),
                                                                 QDir::homePath());
    if (!qemuBinariesPath.isEmpty()) {
        this->m_qemuBinariesPathLineEdit->setText(QDir::toNativeSeparators(qemuBinariesPath));
    }
}

/**
 * @brief Set the qemu-img path
 *
 * Set the qemu-img path
 */
void QEMUBinariesPage::setQemuImgPath()
{
    QString qemuImgPath = QFileDialog::getExistingDirectory(this, tr("Select qemu-img path"),
                                                            QDir::homePath());
    if (!qemuImgPath.isEmpty()) {
        this->m_qemuImgPathLineEdit->setText(QDir::toNativeSeparators(qemuImgPath));
    }
}

/**
 * @brief Set the QEMU machines path
 *
 * Set the QEMU machines path
 */
void QEMUBinariesPage::setQemuMachinesPath()
{
    QString qemuMachinesPath = QFileDialog::getExistingDirectory(this, tr("Select machines path"),
                                                                 QDir::homePath());
    if (!qemuMachinesPath.isEmpty()) {
        this->m_qemuMachinesPathLineEdit->setText(QDir::toNativeSeparators(qemuMachinesPath));
    }
}

/**
 * @brief Save all the data
 *
 * Trigger when the finish button is pressed.
 * Save all the data to the QSettings
 */
bool QEMUBinariesPage::validatePage()
{
    QSettings settings;
    settings.beginGroup("Configuration");
    settings.setValue("firstrunwizard", false);
    settings.setValue("qemuBinaryPath", this->m_qemuBinariesPathLineEdit->text());
    settings.setValue("qemuImgBinaryPath", this->m_qemuImgPathLineEdit->text());
    settings.setValue("machinePath", this->m_qemuMachinesPathLineEdit->text());
#ifdef Q_OS_WIN
    settings.setValue("qemuMonitorHost", this->m_monitorHostnameComboBox->currentText());
    settings.setValue("qemuMonitorPort", this->m_monitorSocketSpinBox->value());
#endif
    settings.endGroup();
    settings.sync();

    return true;
}

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

#ifndef FIRSTRUNWIZARD_H
#define FIRSTRUNWIZARD_H

// Qt
#include <QObject>
#include <QWizard>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QHideEvent>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QPushButton>
#include <QDebug>

class FirstRunWizard : public QWizard {
    Q_OBJECT

    public:
        explicit FirstRunWizard(QWidget *parent = nullptr);
        ~FirstRunWizard();

        enum { Page_QEMUBinaries };

    signals:

    public slots:

    protected:
        virtual void closeEvent(QCloseEvent *event);

    private:
        QMessageBox *m_warningFinishMessageBox;
};

class QEMUBinariesPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit QEMUBinariesPage(QWidget *parent = nullptr);
        ~QEMUBinariesPage();

    signals:

    private slots:

    protected:

    private:
        QGridLayout *m_mainLayout;

        QLabel *m_descriptionLabel;
        QLabel *m_qemuBinariesPathLabel;
        QLabel *m_qemuImgPathLabel;
        QLabel *m_vmsPathLabel;
        QLabel *m_descriptionHostPortLabel;
        QLabel *m_monitorHostnameLabel;
        QLabel *m_monitorSocketPathLabel;

        QLineEdit *m_qemuBinariesPathLineEdit;
        QLineEdit *m_qemuImgPathLineEdit;
        QLineEdit *m_qemuMachinesPathLineEdit;

        QComboBox *m_monitorHostnameComboBox;

        QSpinBox *m_monitorSocketSpinBox;

        QPushButton *m_qemuBinariesPushButton;
        QPushButton *m_qemuImgPathPushButton;
        QPushButton *m_qemuMachinesPathPushButton;
};

#endif // FIRSTRUNWIZARD_H

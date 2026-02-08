// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef IMPORTGENERALPAGE_H
#define IMPORTGENERALPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include <QDebug>

// Local

class ImportGeneralPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ImportGeneralPage(QWidget *parent = nullptr);
        ~ImportGeneralPage();

    signals:

    public slots:

    private slots:
        void selectMachineConfigFile();

    protected:

    private:
        QHBoxLayout *m_machineConfigLayout;
        QVBoxLayout *m_mainLayout;

        QLabel *m_infoLabel;

        QLineEdit *m_machineConfigLineEdit;

        QPushButton *m_machineConfigButton;
};

#endif // IMPORTGENERALPAGE_H

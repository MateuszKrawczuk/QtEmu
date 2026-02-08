// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef IMPORTDESTINATIONPAGE_H
#define IMPORTDESTINATIONPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include <QDebug>

// Local

class ImportDestinationPage : public QWizardPage {
    Q_OBJECT

    public:
        explicit ImportDestinationPage(QWidget *parent = nullptr);
        ~ImportDestinationPage();

    signals:

    public slots:

    private slots:
        void selectMachineDestination();

    protected:

    private:
        QHBoxLayout *m_machineDestinationLayout;
        QVBoxLayout *m_mainLayout;

        QLabel *m_infoLabel;

        QLineEdit *m_machineDestinationLineEdit;

        QPushButton *m_machineDestinationButton;
};

#endif // IMPORTDESTINATIONPAGE_H

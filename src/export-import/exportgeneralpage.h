// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef EXPORTGENERALPAGE_H
#define EXPORTGENERALPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include <QDebug>

class ExportGeneralPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ExportGeneralPage(QWidget *parent = nullptr);
        ~ExportGeneralPage();

    signals:

    public slots:

    private slots:
        void selectExportDestination();

    protected:

    private:
        QHBoxLayout *m_destinationLayout;
        QVBoxLayout *m_mainLayout;

        QLabel *m_infoLabel;

        QLineEdit *m_destinationLineEdit;

        QPushButton *m_destinationButton;
};

#endif // EXPORTGENERALPAGE_H

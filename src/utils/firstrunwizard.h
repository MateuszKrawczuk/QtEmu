// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
#include <QFileDialog>
#include <QDir>
#include <QDebug>

class QEMUBinariesPage;

class FirstRunWizard : public QWizard {
    Q_OBJECT

    public:
        explicit FirstRunWizard(QWidget *parent = nullptr);
        ~FirstRunWizard();

        enum { Page_QEMUBinaries };

    signals:

    public slots:

    private slots:
        void cancelButton();

    protected:
        virtual void closeEvent(QCloseEvent *event);
        virtual void hideEvent(QHideEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);

    private:
        QMessageBox *m_warningFinishMessageBox;
        QEMUBinariesPage *m_qemuBinariesPage;
};

class QEMUBinariesPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit QEMUBinariesPage(QWidget *parent = nullptr);
        ~QEMUBinariesPage();

        bool isWizardComplete();
    signals:

    private slots:
        void setQemuBinariesPath();
        void setDefaultQemuBinariesPath();
        void setQemuMachinesPath();

    protected:

    private:
        QGridLayout *m_mainLayout;

        QLabel *m_descriptionLabel;
        QLabel *m_qemuBinariesPathLabel;
        QLabel *m_vmsPathLabel;
        QLabel *m_descriptionHostPortLabel;
        QLabel *m_monitorHostnameLabel;
        QLabel *m_monitorSocketPathLabel;

        QLineEdit *m_qemuBinariesPathLineEdit;
        QLineEdit *m_qemuMachinesPathLineEdit;

        QComboBox *m_monitorHostnameComboBox;

        QSpinBox *m_monitorSocketSpinBox;

        QPushButton *m_qemuBinariesPushButton;
        QPushButton *m_qemuMachinesPathPushButton;

        // Methods
        bool validatePage();
};

#endif // FIRSTRUNWIZARD_H

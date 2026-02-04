// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGCLOUDINIT_H
#define MACHINECONFIGCLOUDINIT_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>

// Local
#include "../machine.h"

class MachineConfigCloudInit : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigCloudInit(Machine *machine,
                                        QWidget *parent = nullptr);
        ~MachineConfigCloudInit();
        QWidget *m_cloudInitPageWidget;

        // Method
        void saveCloudInitData();

    signals:

    public slots:

    private slots:
        void cloudInitToggled(bool checked);
        void selectSSHKeyFile();
        void loadUserDataFile();

    protected:

    private:
        QVBoxLayout *m_cloudInitPageLayout;
        QFormLayout *m_basicConfigLayout;
        QHBoxLayout *m_sshKeyLayout;
        QHBoxLayout *m_userDataButtonsLayout;

        QCheckBox *m_enableCloudInitCheckBox;
        QGroupBox *m_basicConfigGroupBox;
        QGroupBox *m_advancedConfigGroupBox;

        QLabel *m_hostnameLabel;
        QLineEdit *m_hostnameLineEdit;

        QLabel *m_usernameLabel;
        QLineEdit *m_usernameLineEdit;

        QLabel *m_passwordLabel;
        QLineEdit *m_passwordLineEdit;

        QLabel *m_sshKeyLabel;
        QLineEdit *m_sshKeyLineEdit;
        QPushButton *m_sshKeyBrowseButton;

        QLabel *m_userDataLabel;
        QTextEdit *m_userDataTextEdit;
        QPushButton *m_loadUserDataButton;

        QLabel *m_helpLabel;

        Machine *m_machine;
};

#endif // MACHINECONFIGCLOUDINIT_H

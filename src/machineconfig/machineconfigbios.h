// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGBIOS_H
#define MACHINECONFIGBIOS_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

// Local
#include "../machine.h"
#include "../qemu.h"

class MachineConfigBios : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigBios(Machine *machine,
                                   QEMU *qemuObject,
                                   QWidget *parent = nullptr);
        ~MachineConfigBios();
        QWidget *m_biosPageWidget;

        // Method
        void saveBiosData();

    signals:

    public slots:

    private slots:
        void customBiosToggled(bool checked);
        void selectBiosFile();
        void customBiosDirToggled(bool checked);
        void selectBiosDirectory();

    protected:

    private:
        QVBoxLayout *m_biosPageLayout;
        QHBoxLayout *m_biosFileLayout;
        QHBoxLayout *m_biosDirLayout;

        QCheckBox *m_useCustomBiosCheckBox;
        QLabel *m_biosFileLabel;
        QComboBox *m_biosComboBox;
        QPushButton *m_biosFilePushButton;

        QCheckBox *m_useCustomBiosDirCheckBox;
        QLabel *m_biosDirLabel;
        QLineEdit *m_biosDirLineEdit;
        QPushButton *m_biosDirPushButton;

        Machine *m_machine;
        QEMU *m_qemuObject;
};

#endif // MACHINECONFIGBIOS_H

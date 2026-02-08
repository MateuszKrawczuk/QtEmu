// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

// Qt
#include <QWizardPage>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QGridLayout>
#include <QSettings>
#include <QDir>
#include <QLineEdit>
#include <QUuid>

// Local
#include "../machine.h"
#include "../utils/systemutils.h"

class MachineNamePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNamePage(Machine *machine,
                                 QWidget *parent = nullptr);
        ~MachineNamePage();

    signals:

    public slots:

    private slots:
        void selectOS(int OSSelected);

    protected:

    private:
        void initializePage();
        bool validatePage();
        void cleanupPage();

        QString m_machineFolder;

        QGridLayout *m_mainLayout;

        QLabel *m_descriptionNameLabel;
        QLabel *m_machineNameLabel;
        QLabel *m_OSTypeLabel;
        QLabel *m_OSVersionLabel;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;

        QLineEdit *m_machineNameLineEdit;

        QMessageBox *m_createMachineMessageBox;

        Machine *m_newMachine;
};

#endif // GENERALPAGE_H

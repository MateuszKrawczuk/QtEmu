// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGGENERALTABS_H
#define MACHINECONFIGGENERALTABS_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QLineEdit>

// Local
#include "../machine.h"


class BasicTab: public QWidget {
    Q_OBJECT

    public:
        explicit BasicTab(Machine *machine,
                          bool enableFields,
                          QWidget *parent = nullptr);
        ~BasicTab();
        QString getMachineName() const;
        QString getMachineType() const;
        QString getMachineVersion() const;

    signals:

    public slots:

    private slots:
        void selectOS(QString OSSelected);
        QString getStatusLabel(Machine::States state);

    protected:

    private:
        QVBoxLayout *m_basicTabLayout;
        QFormLayout *m_basicTabFormLayout;

        QLineEdit *m_machineNameLineEdit;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;

        QLabel *m_machineUuidLabel;
        QLabel *m_machineStatusLabel;

        Machine *m_machineConfig;
};

class DescriptionTab: public QWidget {
    Q_OBJECT

    public:
        explicit DescriptionTab(Machine *machine,
                                bool enableFields,
                                QWidget *parent = nullptr);
        ~DescriptionTab();
        QString getMachineDescription() const;

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_descriptionLayout;

        QLabel *m_machineDescLabel;
        QPlainTextEdit *m_machineDescTextEdit;

};

class CustomArgumentsTab: public QWidget {
    Q_OBJECT

    public:
        explicit CustomArgumentsTab(Machine *machine,
                                    bool enableFields,
                                    QWidget *parent = nullptr);
        ~CustomArgumentsTab();
        QString getCustomArguments() const;

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_customArgsLayout;

        QLabel *m_customArgsLabel;
        QLabel *m_customArgsHintLabel;
        QPlainTextEdit *m_customArgsTextEdit;
};

#endif // MACHINECONFIGGENERALTABS_H

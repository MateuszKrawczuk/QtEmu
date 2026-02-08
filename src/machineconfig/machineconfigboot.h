// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGBOOT_H
#define MACHINECONFIGBOOT_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTreeWidget>
#include <QCheckBox>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

// Local
#include "../machine.h"

class MachineConfigBoot : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigBoot(Machine *machine,
                                   QWidget *parent = nullptr);
        ~MachineConfigBoot();
        QWidget *m_bootPageWidget;

        // Method
        void saveBootData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_bootTreeLayout;
        QVBoxLayout *m_bootPageLayout;
        QGridLayout *m_kernelLayout;

        QTreeWidget *m_bootTree;
        QTreeWidgetItem *m_treeItem;

        QCheckBox *m_bootMenuCheckBox;
        QCheckBox *m_kernelBootCheckBox;

        QToolButton *m_moveUpToolButton;
        QToolButton *m_moveDownToolButton;

        QLabel *m_kernelPathLabel;
        QLabel *m_initrdLabel;
        QLabel *m_kernelArgsLabel;

        QLineEdit *m_kernelPathLineEdit;
        QLineEdit *m_initredLineEdit;
        QLineEdit *m_kernelArgsLineEdit;

        QPushButton *m_kernelPathPushButton;
        QPushButton *m_initrdPushButton;

        Machine *m_machine;

        // Methods
        void moveUpButton();
        void moveDownButton();
        void selectEnableKernelBoot(bool enableKernelBoot);
        void setKernelPath();
        void setInitrdPath();
};

#endif // MACHINECONFIGBOOT_H

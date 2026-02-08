// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIAUTILS_H
#define MEDIAUTILS_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

// Local
#include "../machine.h"
#include "../qemu.h"
#include "../utils/systemutils.h"

class NewDiskWizard : public QWizard {
    Q_OBJECT

    public:
        explicit NewDiskWizard(Machine *machine,
                               QEMU *QEMUGlobalObject,
                               Media *media,
                               QWidget *parent = nullptr);
        ~NewDiskWizard();

        enum { Page_Disk };

    signals:

    public slots:

    protected:

    private:

};

class NewDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit NewDiskPage(Machine *machine,
                             QEMU *QEMUGlobalObject,
                             Media *media,
                             QWidget *parent = nullptr);
        ~NewDiskPage();

    signals:

    private slots:
        void selectNameNewDisk();

    protected:

    private:
        QVBoxLayout *m_newDiskLayout;
        QHBoxLayout *m_fileLocationLayout;
        QGridLayout *m_fileSizeLayout;
        QGridLayout *m_diskTypeLayout;

        QGroupBox *m_fileLocationGroupBox;
        QGroupBox *m_fileSizeGroupBox;
        QGroupBox *m_fileTypeGroupBox;

        QLineEdit *m_fileNameLineEdit;

        QDoubleSpinBox *m_diskSpinBox;

        QSlider *m_diskSlider;

        QLabel *m_minDiskLabel;
        QLabel *m_maxDisklabel;
        QLabel *m_memoryLabel;

        QPushButton *m_pathNewDiskPushButton;

        QRadioButton *m_rawRadioButton;
        QRadioButton *m_qcowRadioButton;
        QRadioButton *m_qcow2RadioButton;
        QRadioButton *m_qedRadioButton;
        QRadioButton *m_vmdkRadioButton;
        QRadioButton *m_cloopRadioButton;

        QString m_diskFormat;
        QString m_diskPath;

        Machine *m_machineConfig;
        Media *m_newMedia;
        QEMU *m_qemuGlobalObject;

        // Methods
        bool validatePage();
        QString getExtension();
};

#endif // MEDIAUTILS_H

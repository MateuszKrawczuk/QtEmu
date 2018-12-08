/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

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

        QMessageBox *m_qemuImgNotFoundMessageBox;
        QMessageBox *m_qemuImgNotFinishedMessageBox;
        QMessageBox *m_qemuImgErrorMessageBox;
        QMessageBox *m_qemuImgOkMessageBox;

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

/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef MACHINEWIZARD_H
#define MACHINEWIZARD_H

// Qt
#include <QWizard>
#include <QSettings>
#include <QDir>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QTextBrowser>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QDebug>

// Local
#include "systemutils.h"

class MachineWizard : public QWizard {
    Q_OBJECT

    public:
        explicit MachineWizard(QWidget *parent = 0);
        ~MachineWizard();

    enum { Page_Name, Page_Hardware, Page_Accelerator,
           Page_Memory, Page_Disk, Page_Conclusion };

    signals:

    public slots:

    protected:

    private:

};

class MachineNamePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNamePage(QWidget *parent = 0);
        ~MachineNamePage();

    signals:

    public slots:
        void selectOS(int OSSelected);

    protected:

    private:
        bool validatePage();

        QGridLayout *mainLayout;

        QLabel *descriptionNameLabel;
        QLabel *machineNameLabel;
        QLabel *OSTypeLabel;
        QLabel *OSVersionLabel;

        QComboBox *OSType;
        QComboBox *OSVersion;

        QLineEdit *machineNameLineEdit;

        QMessageBox *createMachineMessageBox;
};

class MachineHardwarePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineHardwarePage(QWidget *parent = 0);
        ~MachineHardwarePage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *hardwareLayout;

        QTabWidget *hardwareTabWidget;
};

class ProcessorTab: public QWidget {
    Q_OBJECT

    public:
        explicit ProcessorTab(QWidget *parent = 0);
        ~ProcessorTab();
    signals:

    public slots:

    protected:

    private:
        QHBoxLayout *CPUTypeLayout;
        QHBoxLayout *CPUCountLayout;
        QHBoxLayout *socketLayout;
        QHBoxLayout *maxHotCPUsLayout;
        QVBoxLayout *CPUSettingsLayout;
        QVBoxLayout *processorLayout;

        QComboBox *CPUType;

        QGroupBox *CPUSettings;

        QLabel *CPUTypeLabel;
        QLabel *CPUCountLabel;
        QLabel *coresSocketLabel;
        QLabel *socketCountLabel;
        QLabel *threadsCoreLabel;
        QLabel *maxHotCPULabel;

        QSpinBox *CPUCountSpinBox;
        QSpinBox *coresSocketSpinBox;
        QSpinBox *socketCountSpinBox;
        QSpinBox *threadsCoreSpinBox;
        QSpinBox *maxHotCPUSpinBox;
};

class GraphicsTab: public QWidget {
    Q_OBJECT

    public:
        explicit GraphicsTab(QWidget *parent = 0);
        ~GraphicsTab();
    signals:

    public slots:

    protected:

    private:
        QGridLayout *graphicsLayout;

        QComboBox *GPUType;
        QComboBox *keyboard;

        QLabel *GPUTypeLabel;
        QLabel *keyboardLabel;
};

class AudioTab: public QWidget {
    Q_OBJECT

    public:
        explicit AudioTab(QWidget *parent = 0);
        ~AudioTab();
    signals:

    public slots:

    protected:

    private:
        QGridLayout *audioLayout;

        QCheckBox *creativeCheck;
        QCheckBox *ensoniqCheck;
        QCheckBox *intelAC97Check;
        QCheckBox *yamahaCheck;
        QCheckBox *gravisCheck;
        QCheckBox *CS4231ACheck;
        QCheckBox *intelHDACheck;
        QCheckBox *pcSpeakerCheck;
};

class NetworkTab: public QWidget {
    Q_OBJECT

    public:
        explicit NetworkTab(QWidget *parent = 0);
        ~NetworkTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *networkLayout;

        QRadioButton *withNetworkRadio;
        QRadioButton *withoutNetworkRadio;

};

class MachineAcceleratorPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineAcceleratorPage(QWidget *parent = 0);
        ~MachineAcceleratorPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *acceleratorLayout;

        QTabWidget *acceleratorTabWidget;

};

class KVMTab: public QWidget {
    Q_OBJECT

    public:
        explicit KVMTab(QWidget *parent = 0);
        ~KVMTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *kvmLayout;

        QRadioButton *kvmRadioButton;

        QLabel *kvmDescriptionLabel;
        QLabel *kvmURLLabel;

};

class TCGTab: public QWidget {
    Q_OBJECT

    public:
        explicit TCGTab(QWidget *parent = 0);
        ~TCGTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *tcgLayout;

        QRadioButton *tcgRadioButton;

        QLabel *tcgDescriptionLabel;
        QLabel *tcgURLLabel;

};

class HAXMTab: public QWidget {
    Q_OBJECT

    public:
        explicit HAXMTab(QWidget *parent = 0);
        ~HAXMTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *haxmLayout;

        QRadioButton *haxmRadioButton;

        QLabel *haxmDescriptionLabel;
        QLabel *haxmURLLabel;
};

class MachineMemoryPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineMemoryPage(QWidget *parent = 0);
        ~MachineMemoryPage();

    signals:

    public slots:

    protected:

    private:
        QGridLayout *machineMemoryLayout;

        QSpinBox *memorySpinBox;
        QSlider *memorySlider;

        QLabel *descriptionMemoryLabel;
        QLabel *spinBoxMemoryLabel;
        QLabel *minMemoryLabel;
        QLabel *maxMemorylabel;
};

class MachineDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineDiskPage(QWidget *parent = 0);
        ~MachineDiskPage();

    signals:

    public slots:
        void useExistingDiskToggle(bool toggled);

    protected:

    private:
        QVBoxLayout *machineDiskLayout;
        QHBoxLayout *useOldDiskLayout;

        QRadioButton *noDiskRadio;
        QRadioButton *createDiskRadio;
        QRadioButton *useExistingDiskRadio;

        QPushButton *pathNewDiskPushButton;

        QLineEdit *hardDiskPathLineEdit;

        QLabel *machineDiskLabel;
        QLabel *machineDiskInfoLabel;
        QLabel *machineDiskSizeLabel;
};

class MachineConclusionPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineConclusionPage(QWidget *parent = 0);
        ~MachineConclusionPage();

    signals:

    public slots:

    protected:

    private:
        QFormLayout *conclusionLayout;

        QLabel *conclusionCPULabel;


};

#endif // MACHINEWIZARD_H

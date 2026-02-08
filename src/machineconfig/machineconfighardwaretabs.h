// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGHARDWARETABS_H
#define MACHINECONFIGHARDWARETABS_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLineEdit>

// Local
#include "../components/customfilter.h"
#include "../machine.h"

class ProcessorConfigTab: public QWidget {
    Q_OBJECT

    public:
        explicit ProcessorConfigTab(Machine *machine,
                                    bool enableFields,
                                    QWidget *parent = nullptr);
        ~ProcessorConfigTab();

        // Methods
        QString getCPUType();
        int getCPUCount();

    signals:

    public slots:

    protected:

    private:
        QHBoxLayout *m_CPUTypeLayout;
        QHBoxLayout *m_CPUCountLayout;
        QVBoxLayout *m_CPUSettingsLayout;
        QVBoxLayout *m_processorLayout;

        QComboBox *m_CPUType;

        QGroupBox *m_CPUSettings;

        QLabel *m_CPUTypeLabel;
        QLabel *m_CPUCountLabel;

        QSpinBox *m_CPUCountSpinBox;

};

class GraphicsConfigTab: public QWidget {
    Q_OBJECT

    public:
        explicit GraphicsConfigTab(Machine *machine,
                                   bool enableFields,
                                   QWidget *parent = nullptr);
        ~GraphicsConfigTab();

        // Methods
        QString getGPUType();
        QString getKeyboardLayout();

    signals:

    public slots:

    protected:

    private:
        QHBoxLayout *m_gpuLayout;
        QHBoxLayout *m_keyboardLayout;
        QVBoxLayout *m_graphicsLayout;

        QComboBox *m_GPUType;
        QComboBox *m_keyboard;

        QLabel *m_GPUTypeLabel;
        QLabel *m_keyboardLabel;

};

class RamConfigTab: public QWidget {
    Q_OBJECT

    public:
        explicit RamConfigTab(Machine *machine,
                              bool enableFields,
                              QWidget *parent = nullptr);
        ~RamConfigTab();

        // Methods
        int getAmountRam();

    signals:

    public slots:

    protected:

    private:
        QGridLayout *m_machineMemoryLayout;

        QSpinBox *m_memorySpinBox;
        QSlider *m_memorySlider;

        QLabel *m_descriptionMemoryLabel;
        QLabel *m_spinBoxMemoryLabel;
        QLabel *m_minMemoryLabel;
        QLabel *m_maxMemorylabel;
};

class MachineTypeTab : public QWidget {
    Q_OBJECT

    public:
        explicit MachineTypeTab(Machine *machine,
                                bool enableFields,
                                QEMU *qemuObject,
                                QWidget *parent = nullptr);
        ~MachineTypeTab();

        // Methods
        QString getMachineType();

    signals:

    public slots:

    private slots:
        void textFilterChanged();

    protected:

    private:
        QGridLayout *machinePageLayout;

        CustomFilter *customFilter;

        QTreeView *machinesTypesTreeView;

        QLabel *filterLabel;

        QLineEdit *filterLineEdit;

        Machine *m_machine;
        QEMU *m_qemuObject;

        // Methods
        void setMachines();
        void addMachine(QAbstractItemModel *model, const QString &machine, const QString &description);
        QMap<QString, QString> getMachineTypesFromQEMU(QEMU *qemuObject);
        bool isDeprecatedVersion(const QString &description);
};

#endif // MACHINECONFIGHARDWARETABS_H

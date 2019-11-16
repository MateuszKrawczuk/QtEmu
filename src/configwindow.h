/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2019 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

// Qt
#include <QWidget>
#include <QSettings>
#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QString>
#include <QPlainTextEdit>
#include <QNetworkProxy>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QCloseEvent>
#include <QTableWidget>
#include <QToolButton>
#include <QFileDialog>
#include <QSpinBox>

#include <QDebug>

// Local
#include "qemu.h"

class ConfigWindow : public QWidget {
    Q_OBJECT

    public:
        explicit ConfigWindow(QEMU *QEMUGlobalObject,
                              QWidget *parent = nullptr);
        ~ConfigWindow();

    signals:

    public slots:

    private slots:
        void closeEvent (QCloseEvent *event);
        void toggleUpdate(bool updateState);
        void pushStableVersion(bool release);
        void pushBetaVersion(bool release);
        void pushDevelopmentVersion(bool release);
        void setLanguageLabel(QString language);
        void setAuthorsLabel(int languagePosition);
        void toggleServerPort(int proxyOption);
        void toggleAuth(bool authState);
        void setMachinePath();
        void cancelButton();
        void saveSettings();
        void loadSettings();
        void setPathBinaries();
        void findBinaries();
        void binaryPathChanged(const QString binaryPath);

    protected:

    private:
        // Main layout
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_topLayout;
        QHBoxLayout *m_buttonsLayout;

        QListWidget *m_optionsListWidget;
        QStackedWidget *m_categoriesStackedWidget;

        QPushButton *m_saveButton;
        QPushButton *m_closeButton;

        QAction *m_closeAction;

        // General page
        QGroupBox *m_machinePathGroup;
        QHBoxLayout *m_machinePathLayout;
        QHBoxLayout *m_machineSocketLayout;
        QHBoxLayout *m_machinePortSocketLayout;
        QVBoxLayout *m_groupLayout;
        QVBoxLayout *m_generalPageLayout;
        QWidget *m_generalPageWidget;

        QLabel *m_machinePathLabel;
        QLabel *m_monitorHostnameLabel;
        QLabel *m_monitorSocketPathLabel;

        QLineEdit *m_machinePathLineEdit;

        QPushButton *m_machinePathButton;

        QString m_machinePath;
        QString sm_tartCommand;

        QFileDialog *m_machinePathDialog;
        QFileDialog *m_startCommandDialog;

        QComboBox *m_monitorHostnameComboBox;

        QSpinBox *m_monitorSocketSpinBox;

        // Update QtEmu page
        QFormLayout *m_updatePageLayout;
        QVBoxLayout *m_updateRadiosLayout;
        QWidget *m_updatePageWidget;

        QCheckBox *m_updateCheckBox;

        QGroupBox *m_updatesGroup;

        QRadioButton *m_stableReleaseRadio;
        QRadioButton *m_betaReleaseRadio;
        QRadioButton *m_developmentRelaseRadio;

        QString m_releaseString;

        // Language
        QVBoxLayout *m_languagePageLayout;
        QHBoxLayout *m_languageHLayout;
        QListWidget *m_languagesListView;

        QLabel *m_languageLabel;
        QLabel *m_languageDescription;
        QLabel *m_languageAuthors;

        QString m_languageISOCode;
        int m_languagePos;

        QWidget *m_languagePageWidget;

        // Start
        // QtEmu 2.x
        /*QVBoxLayout *m_startPageLayout;
        QWidget *m_startPageWidget;

        QPlainTextEdit *m_beforeStart;
        QPlainTextEdit *m_afterExit;

        QLabel *m_beforeStartLabel;
        QLabel *m_afterExitLabel;*/

        // Proxy
        QFormLayout *m_proxyPageLayout;
        QWidget *m_proxyPageWidget;

        QComboBox *m_proxyOptions;

        QLineEdit *m_serverNameProxy;
        QLineEdit *m_portProxy;
        QLineEdit *m_userProxy;
        QLineEdit *m_passwordProxy;

        QCheckBox *m_useAuth;

        // QEMU
        QHBoxLayout *m_binaryLabelLayout;
        QHBoxLayout *m_binaryLayout;
        QHBoxLayout *m_QEMUPathLayout;
        QVBoxLayout *m_QEMULayout;
        QWidget *m_QEMUPageWidget;

        QLabel *m_findBinaryLabel;
        QLineEdit *m_binaryPathLineEdit;

        QLabel *m_QEMUImgLabel;

        QTableWidget *m_binariesTableWidget;

        QToolButton *m_binariesPathToolButton;
        QToolButton *m_searchBinariesToolButton;

        QEMU *m_QEMUObject;

        // Methods
        void createGeneralPage();
        void createUpdatePage();
        void createLanguagePage();
        void createStartPage();
        void createProxyPage();
        void createQEMUPage();
        void insertBinariesInTree();
};

#endif // CONFIGWINDOW_H

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
#include <QWidget>
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

#include <QDebug>

class ConfigWindow : public QWidget {
    Q_OBJECT

    public:
        explicit ConfigWindow(QWidget *parent = 0);
        ~ConfigWindow();

        void createGeneralPage();
        void createInputPage();
        void createUpdatePage();
        void createLanguagePage();
        void createStartPage();
        void createProxyPage();

    signals:

    public slots:
        void toggleUpdate(bool updateState);
        void pushStableVersion(bool release);
        void pushBetaVersion(bool release);
        void pushDevelopmentVersion(bool release);
        void setLanguageLabel(QString language);
        void setAuthorsLabel(int languagePosition);
        void toggleUserPassword(int proxyOption);
        void toggleAuth(bool authState);
        void setMachinePath();
        void saveSettings();
        void loadSettings();

    protected:

    private:
        // Main layout
        QVBoxLayout *mainLayout;
        QHBoxLayout *topLayout;
        QHBoxLayout *buttonsLayout;

        QListWidget *optionsListWidget;
        QStackedWidget *categoriesStackedWidget;

        QPushButton *saveButton;
        QPushButton *closeButton;

        QAction *closeAction;

        // General page
        QGroupBox *machinePathGroup;
        QGroupBox *startCommandGroup;
        QHBoxLayout *machinePathLayout;
        QHBoxLayout *startCommandLayout;
        QVBoxLayout *generalPageLayout;
        QWidget *generalPageWidget;

        QLabel *machinePathLabel;
        QLabel *startCommandLabel;

        QLineEdit *machinePathLineEdit;
        QLineEdit *startCommandLineEdit;

        QPushButton *machinePathButton;

        QString machinePath;
        QString startCommand;

        QFileDialog *machinePathDialog;
        QFileDialog *startCommandDialog;

        // Input page
        QVBoxLayout *inputPageLayout;
        QWidget *inputPageWidget;

        // Update QtEmu page
        QFormLayout *updatePageLayout;
        QVBoxLayout *updateRadiosLayout;
        QWidget *updatePageWidget;

        QCheckBox *updateCheckBox;

        QGroupBox *updatesGroup;

        QRadioButton *stableReleaseRadio;
        QRadioButton *betaReleaseRadio;
        QRadioButton *developmentRelaseRadio;

        QString releaseString;

        // Language
        QVBoxLayout *languagePageLayout;
        QHBoxLayout *languageHLayout;
        QListWidget *languagesListView;

        QLabel *languageLabel;
        QLabel *languageDescription;
        QLabel *languageAuthors;

        QString languageISOCode;
        int languagePos;

        QWidget *languagePageWidget;

        // Start
        QVBoxLayout *startPageLayout;
        QWidget *startPageWidget;

        QPlainTextEdit *beforeStart;
        QPlainTextEdit *afterExit;

        QLabel *beforeStartLabel;
        QLabel *afterExitLabel;

        // Proxy
        QFormLayout *proxyPageLayout;
        QWidget *proxyPageWidget;

        QComboBox *proxyOptions;

        QLineEdit *serverNameProxy;
        QLineEdit *portProxy;
        QLineEdit *userProxy;
        QLineEdit *passwordProxy;

        QCheckBox *useAuth;

};

#endif // CONFIGWINDOW_H

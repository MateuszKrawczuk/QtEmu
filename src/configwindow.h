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
        void setLanguageLabel(QString language);
        void setAuthorsLabel(int languagePosition);

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
        QVBoxLayout *generalPageLayout;
        QWidget *generalPageWidget;
        QLabel *defaultMachineFolder;

        // Input page
        QVBoxLayout *inputPageLayout;
        QWidget *inputPageWidget;

        // Update QtEmu page
        QVBoxLayout *updatePageLayout;
        QWidget *updatePageWidget;

        // Language
        QVBoxLayout *languagePageLayout;
        QHBoxLayout *languageHLayout;
        QListWidget *languagesListView;

        QLabel *languageLabel;
        QLabel *languageDescription;
        QLabel *languageAuthors;

        QWidget *languagePageWidget;

        // Start
        QVBoxLayout *startPageLayout;
        QWidget *startPageWidget;

        QPlainTextEdit *beforeStart;
        QPlainTextEdit *afterExit;

        QLabel *beforeStartLabel;
        QLabel *afterExitLabel;

        // Proxy
        QVBoxLayout *proxyPageLayout;
        QWidget *proxyPageWidget;

};

#endif // CONFIGWINDOW_H

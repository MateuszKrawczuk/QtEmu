/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QCloseEvent>
#include <QHideEvent>
#include <QTabWidget>
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QTextBrowser>
#include <QDebug>

class AboutTab : public QWidget {
    Q_OBJECT

    public:
        explicit AboutTab(QWidget *parent = nullptr);
        ~AboutTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_mainLayout;
        QLabel *m_about;
        QLabel *m_copyright;
        QLabel *m_urlSite;
};

class AuthorsTab : public QWidget {
    Q_OBJECT

    public:
        explicit AuthorsTab(QWidget *parent = nullptr);
        ~AuthorsTab();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_mainLayout;
        QTextBrowser *m_authorsBrowser;
};

class LicenseTab : public QWidget {
    Q_OBJECT

    public:
        explicit LicenseTab(QWidget *parent = nullptr);
        ~LicenseTab();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_mainLayout;
        QTextBrowser *m_licenseBrowser;
};

class AboutWidget : public QWidget {
    Q_OBJECT

    public:
        explicit AboutWidget(QWidget *parent = nullptr);
        ~AboutWidget();

    signals:

    public slots:

    protected:
        virtual void closeEvent(QCloseEvent *event);
        virtual void hideEvent(QHideEvent *event);

    private:
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_iconLayout;
        QPushButton *m_closeButton;
        QAction *m_closeAction;
        QTabWidget *m_tabWidget;
        QLabel *m_qtemuIcon;
        QLabel *m_qtemuAppInfo;
};

#endif // ABOUTWIDGET_H

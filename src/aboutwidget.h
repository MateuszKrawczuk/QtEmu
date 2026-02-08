// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

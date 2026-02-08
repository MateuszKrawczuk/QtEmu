// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HELPWIDGET_H
#define HELPWIDGET_H

// Qt

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QAction>
#include <QCloseEvent>
#include <QHideEvent>
#include <QSettings>

#include <QDebug>

class HelpWidget : public QWidget {
    Q_OBJECT

    public:
        explicit HelpWidget(QWidget *parent = nullptr);
        ~HelpWidget();

    signals:

    public slots:

    protected:
        virtual void closeEvent(QCloseEvent *event);
        virtual void hideEvent(QHideEvent *event);

    private:
        QVBoxLayout *m_mainLayout;
        QTextBrowser *m_helpTextBrowser;

        QPushButton *m_closeButton;
        QAction *m_closeAction;

};

#endif // HELPWIDGET_H

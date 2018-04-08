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
        explicit HelpWidget(QWidget *parent = 0);
        ~HelpWidget();

    signals:

    public slots:

    protected:
        virtual void closeEvent(QCloseEvent *event);
        virtual void hideEvent(QHideEvent *event);

    private:
        QVBoxLayout *mainLayout;
        QTextBrowser *helpTextBrowser;

        QPushButton *closeButton;
        QAction *closeAction;

};

#endif // HELPWIDGET_H

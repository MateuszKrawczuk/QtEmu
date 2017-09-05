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

// Qt

// Local
#include "aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) : QWidget(parent) {
    this -> setWindowTitle(tr("About") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/icon/32x32/qtemu.png")));
    this -> setWindowFlag(Qt::Window);
    this -> setFixedSize(625, 320);


    /*closeButton = new QPushButton(QIcon::fromTheme("window-close", QIcon(":/images/close.png")),
                                  tr("&Close"),
                                  this);

    connect(closeButton, &QAbstractButton::clicked,
            this, &QWidget::hide);

    QList<QKeySequence> closeShortcuts;
    closeShortcuts << QKeySequence(Qt::Key_Escape);
    closeAction = new QAction(this);
    closeAction -> setShortcuts(closeShortcuts);
    connect(closeAction, &QAction::triggered,
            this, &QWidget::hide);
    this -> addAction(closeAction);

    */

    tabWidget = new QTabWidget(this);
    tabWidget -> addTab(new AboutTab(), tr("About"));
    tabWidget -> addTab(new AuthorsTab(), tr("Authors"));
    tabWidget -> addTab(new LicenseTab(), tr("License"));

    mainLayout = new QVBoxLayout(this);

    iconLayout = new QHBoxLayout(this);
    iconLayout -> setContentsMargins(0, 12, 0, 0);

    qtemuIcon = new QLabel(this);
    qtemuIcon -> setPixmap(QPixmap(":/icon/32x32/qtemu.png"));
    iconLayout -> addWidget(qtemuIcon, 0, Qt::AlignTop);

    qtemuAppInfo = new QLabel(QString("<h3><strong>%1 v%2</strong></h3>")
                              .arg(qApp -> applicationName())
                              .arg(qApp -> applicationVersion()), this);
    qtemuAppInfo->setContentsMargins(10, 0, 0, 0);

    iconLayout -> addWidget(qtemuAppInfo, 1, Qt::AlignTop);
    mainLayout -> addItem(iconLayout);
    mainLayout -> addWidget(tabWidget);

    this -> setLayout(mainLayout);

    qDebug() << "AboutWidget created";
}

AboutWidget::~AboutWidget() {
    qDebug() << "AboutWidget destroyed";
}

void AboutWidget::closeEvent(QCloseEvent *event) {
    this -> hide();
    event -> ignore();
}

void AboutWidget::hideEvent(QHideEvent *event) {
    event -> accept();
}

AboutTab::AboutTab(QWidget *parent) : QWidget(parent) {




    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    this -> setLayout(mainLayout);

}

AuthorsTab::AuthorsTab(QWidget *parent) : QWidget(parent) {




    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    this -> setLayout(mainLayout);

}

LicenseTab::LicenseTab(QWidget *parent) : QWidget(parent) {




    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    this -> setLayout(mainLayout);

}

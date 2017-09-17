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
#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) : QWidget(parent) {
    this -> setWindowTitle(tr("Preferences") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/icon/32x32/qtemu.png")));
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::ApplicationModal);
    this->setMinimumSize(640, 520);

    QSettings settings;

    settings.beginGroup("Configuration");


}

ConfigWindow::~ConfigWindow() {
    qDebug() << "ConfigWindow destroyed";
}

void ConfigWindow::createGeneralPage() {

}

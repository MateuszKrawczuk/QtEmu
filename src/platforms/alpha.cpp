/*
 * This file is part of QtEmu project.
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

#include "alpha.h"

// Cpus
/*
  ev4-alpha-cpu
  ev5-alpha-cpu
  ev56-alpha-cpu
  ev6-alpha-cpu
  ev67-alpha-cpu
  ev68-alpha-cpu
  pca56-alpha-cpu
*/

/**
 * @brief Alpha platforms
 * @param parent, parent widget
 *
 * Alpha object with properties of the Alpha platform
 */
alpha::alpha(QObject *parent) : QObject(parent)
{
    qDebug() << "alpha object created";
}

alpha::~alpha()
{
    qDebug() << "alpha object destroyed";
}

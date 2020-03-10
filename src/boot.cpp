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

#include "boot.h"

/**
 * @brief Boot object
 *
 * Boot object
 */
Boot::Boot(QObject *parent) : QObject(parent)
{
    qDebug() << "Boot object created";
}

Boot::~Boot()
{
    qDebug() << "Boot object destroyed";
}

/**
 * @brief Get if the boot menu is enabled
 * @return true if boot menu is enabled
 *
 * Get if the boot menu is enabled
 */
bool Boot::bootMenu() const
{
    return m_bootMenu;
}

/**
 * @brief Enable the boot menu
 * @param bootMenu, true enable the boot menu
 *
 * Enable the boot menu
 */
void Boot::setBootMenu(bool bootMenu)
{
    m_bootMenu = bootMenu;
}

/**
 * @brief Get if the kernel boot is enabled
 * @return true if kernel boot is enabled
 *
 * Get if the kernel boot is enabled
 */
bool Boot::kernelBootEnabled() const
{
    return m_kernelBootEnabled;
}

/**
 * @brief Enable the kernel boot
 * @param kernelBootEnabled, true enable the kernel boot
 *
 * Enable the kernel boot
 */
void Boot::setKernelBootEnabled(bool kernelBootEnabled)
{
    m_kernelBootEnabled = kernelBootEnabled;
}

/**
 * @brief Get the kernel path
 *
 * Get the kernel path
 */
QString Boot::kernelPath() const
{
    return m_kernelPath;
}

/**
 * @brief Set the kernel path
 * @param kernelPath, kernel path
 *
 * Set the kernel path
 */
void Boot::setKernelPath(const QString &kernelPath)
{
    m_kernelPath = kernelPath;
}

/**
 * @brief Get the initrd path
 *
 * Get the initrd path
 */
QString Boot::initrdPath() const
{
    return m_initrdPath;
}

/**
 * @brief Set the initrd path
 * @param initrdPath, initrd path
 *
 * Set the initrd path
 */
void Boot::setInitrdPath(const QString &initrdPath)
{
    m_initrdPath = initrdPath;
}

/**
 * @brief Get the kernel args
 *
 * Get the kernel args
 */
QString Boot::kernelArgs() const
{
    return m_kernelArgs;
}

/**
 * @brief Set the kernel args
 * @param kernelArgs, kernel args
 *
 * Set the kernel args
 */
void Boot::setKernelArgs(const QString &kernelArgs)
{
    m_kernelArgs = kernelArgs;
}

/**
 * @brief Get the boot order
 * @return list with the boot order
 *
 * Get the boot order
 */
QStringList Boot::bootOrder() const
{
    return m_bootOrder;
}

/**
 * @brief Set the boot order
 * @param bootOrder, list with the new boot order
 *
 * Set the boot order
 */
void Boot::setBootOrder(const QStringList &bootOrder)
{
    m_bootOrder = bootOrder;
}

/**
 * @brief Add one option to the boot order
 * @param bootOrder, option to be added
 *
 * Add one option to the boot order
 */
void Boot::addBootOrder(const QString bootOrder)
{
    if (!this->m_bootOrder.contains(bootOrder)) {
        this->m_bootOrder.append(bootOrder);
    }
}

/**
 * @brief Remove one option in the boot order
 * @param bootOrder, option to be removed
 *
 * Remove one option in the boot order
 */
void Boot::removeBootOrder(const QString bootOrder)
{
    if (this->m_bootOrder.contains(bootOrder)) {
        this->m_bootOrder.removeOne(bootOrder);
    }
}

/**
 * @brief Remove all options in the boot order
 *
 * Remove all options in the boot order
 */
void Boot::removeAllBootOrder()
{
    this->m_bootOrder.clear();
}

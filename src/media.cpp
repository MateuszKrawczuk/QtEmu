/*
 * This file is part of QtEmu project.
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

// Local
#include "media.h"

/**
 * @brief Media object
 *
 * Media object for hdd, cdrom and floppy
 */
Media::Media()
{
    qDebug() << "Media object created";
}

Media::~Media()
{
    qDebug() << "Media object destroyed";
}

/**
 * @brief Get the media name
 * @return media name
 *
 * Get the media names
 */
QString Media::name() const
{
    return m_name;
}

/**
 * @brief Set the media name
 * @param mediaName, new name for the media
 *
 * Set the media name
 */
void Media::setName(const QString &name)
{
    m_name = name;
}

/**
 * @brief Get the media path
 * @return media path
 *
 * Get the media path
 */
QString Media::path() const
{
    return m_path;
}

/**
 * @brief Set the new media path
 * @param mediaPath, new media path
 *
 * Set the new media path
 */
void Media::setPath(const QString &path)
{
    m_path = path;
}

/**
 * @brief Get the media size
 * @return media size
 *
 * Get the media size
 */
qlonglong Media::size() const
{
    return m_size;
}

/**
 * @brief Set the media size
 * @param mediaSize, media size
 *
 * Set the media size
 */
void Media::setSize(const qlonglong &size)
{
    m_size = size;
}

/**
 * @brief Get the media type
 * @return media type
 *
 * Get the media type
 */
QString Media::type() const
{
    return m_type;
}

/**
 * @brief Set the media type
 * @param mediaType, new media type
 *
 * Set the media type
 * Ex: ide, scsi...
 */
void Media::setType(const QString &type)
{
    m_type = type;
}

/**
 * @brief Get the media format
 * @return media format
 *
 * Get the media format
 * Ex: qcow, qcow2, raw...
 */
QString Media::format() const
{
    return m_format;
}

/**
 * @brief Set the media format
 * @param mediaFormat, new media format
 *
 * Set the media format
 */
void Media::setFormat(const QString &format)
{
    m_format = format;
}

/**
 * @brief Get the media interface
 * @return media interface
 *
 * Get the media interface
 * Ex: ide, scsi...
 */
QString Media::driveInterface() const
{
    return m_driveInterface;
}

/**
 * @brief Set the new media interface
 * @param mediaInterface, media interface
 *
 * Set the new media interface
 */
void Media::setDriveInterface(const QString &driveInterface)
{
    m_driveInterface = driveInterface;
}

/**
 * @brief Get the media cache
 * @return the media cache
 *
 * Get the media cache
 * Ex: none, writeback...
 */
QString Media::cache() const
{
    return m_cache;
}

/**
 * @brief Set the media cache
 * @param mediaCache, new media cache
 *
 * Set the media cache
 */
void Media::setCache(const QString &cache)
{
    m_cache = cache;
}

/**
 * @brief Get the media IO
 * @return media IO
 *
 * Get the media IO
 * Ex: threads, native...
 */
QString Media::IO() const
{
    return m_IO;
}

/**
 * @brief Set the media IO
 * @param mediaIO, new media IO
 *
 * Set the media IO
 */
void Media::setIO(const QString &IO)
{
    m_IO = IO;
}

/**
 * @brief Get the uuid of the media
 * @return the uuid
 *
 * Get the uuid of the media
 */
QUuid Media::uuid() const
{
    return m_uuid;
}

/**
 * @brief Set the uuid of the media
 * @param uuid, set the new uuid
 *
 * Set the uuid of the media
 */
void Media::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
}

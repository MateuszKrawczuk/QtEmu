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

#ifndef MEDIA_H
#define MEDIA_H

// Qt
#include <QObject>
#include <QUuid>
#include <QDebug>

class Media: public QObject {
    Q_OBJECT

    public:
        explicit Media(QObject *parent = nullptr);
        ~Media();

        QString name() const;
        void setName(const QString &name);

        QString path() const;
        void setPath(const QString &path);

        qlonglong size() const;
        void setSize(const qlonglong &size);

        QString type() const;
        void setType(const QString &type);

        QString format() const;
        void setFormat(const QString &format);

        QString driveInterface() const;
        void setDriveInterface(const QString &driveInterface);

        QString cache() const;
        void setCache(const QString &cache);

        QString IO() const;
        void setIO(const QString &IO);

        QUuid uuid() const;
        void setUuid(const QUuid &uuid);

    protected:

    private:
        QString m_name;
        QString m_path;
        qlonglong m_size;
        QString m_type;
        QString m_format;
        QString m_driveInterface;
        QString m_cache;
        QString m_IO;
        QUuid m_uuid;
};

#endif // MEDIA_H

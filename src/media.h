// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

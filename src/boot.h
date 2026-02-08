// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef BOOT_H
#define BOOT_H

// Qt
#include <QObject>
#include <QDebug>

class Boot: public QObject {
    Q_OBJECT

    public:
        explicit Boot(QObject *parent = nullptr);
        ~Boot();

        bool bootMenu() const;
        void setBootMenu(bool bootMenu);

        bool kernelBootEnabled() const;
        void setKernelBootEnabled(bool kernelBootEnabled);

        QString kernelPath() const;
        void setKernelPath(const QString &kernelPath);

        QString initrdPath() const;
        void setInitrdPath(const QString &initrdPath);

        QString kernelArgs() const;
        void setKernelArgs(const QString &kernelArgs);

        QStringList bootOrder() const;
        void setBootOrder(const QStringList &bootOrder);

        // Methods
        void addBootOrder(const QString bootOrder);
        void removeBootOrder(const QString bootOrder);
        void removeAllBootOrder();

    protected:

    private:
        bool m_bootMenu;
        bool m_kernelBootEnabled;
        QString m_kernelPath;
        QString m_initrdPath;
        QString m_kernelArgs;
        QStringList m_bootOrder;

};

#endif // BOOT_H

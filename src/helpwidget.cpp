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

// Local
#include "helpwidget.h"

/**
 * @brief Help window
 * @param parent, widget parent
 *
 * Help window with a brief manual about QtEmu
 */
HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(tr("Quick Help") + " - QtEmu");
    this->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    this->setWindowFlag(Qt::Window);
    this->setMinimumSize(100, 500);

    QSettings settings;
    this->resize(settings.value("HelpWidget/helpWindowSize",
                                QSize(580, 700)).toSize());

    QString helpText;

    // Index
    const QString sectionOverview = tr("Overview");
    const QString sectionNewVM    = tr("Create a new VM - step by step");
    const QString sectionToolbar  = tr("Toolbar");
    const QString sectionOptions  = tr("Options");
    const QString sectionOther    = tr("Other");

    helpText.append("<h1>" + tr("Index") + "</h1>"
                    "<ul>"
                        "<li>"
                            "<a href='#overview'>" + sectionOverview + "</a>"
                        "</li>"
                        "<li>"
                            "<a href='#newVM'>"    + sectionNewVM    + "</a>"
                        "</li>"
                        "<li>"
                            "<a href='#toolbar'>"  + sectionToolbar  + "</a>"
                        "</li>"
                        "<li>"
                            "<a href='#options'>"  + sectionOptions  + "</a>"
                        "</li>"
                        "<li>"
                            "<a href='#other'>"    + sectionOther    + "</a>"
                        "</li>"
                    "</ul>"
                    "<hr />"
                    );

    helpText.append("<h2><a id='overview'>" + sectionOverview + "</h2></a>");

    helpText.append("<p>" +
                    tr("QtEmu is a graphical user interface for QEMU. "
                       "It has the ability to run virtual operating systems "
                       "on native systems. "
                       "This way you can easily test a new operating system or "
                       "try a Live CD on your system without any troubles and "
                       "dangers.") +
                    "</p>");
    helpText.append("<br />");

    helpText.append("<h2><a id='newVM'>" + sectionNewVM + "</h2></a>");
    helpText.append("<ol>"
                        "<li>" +
                            tr("Click on the \"New virtual machine\" button, "
                               "either in Toolbar or Main tab. A wizard opens.") +
                        "</li>"
                        "<li>" +
                            tr("Select in the combobox the operation system you "
                               "want to install. If it is not listed there, select \"Other\".") +
                        "</li>"
                        "<li>" +
                            tr("There is already a name for the virtual machine proposed. "
                               "If you have selected \"Other\", your should now enter the correct name.") +
                        "</li>"
                        "<li>" +
                            tr("When you change the name, the path adjusts automatically itself.") +
                        "</li>"
                        "<li>" +
                            tr("Now you can specify how much space should be allocated for "
                               "the virtual disk image. Do not specify a bigger size than there "
                               "is space available on the selected real hard disk. "
                               "Click \"Finish\". The wizard closes and the new created machine opens.") +
                        "</li>"
                        "<li>" +
                            tr("Now you are able to configure the new virtual machine.") +
                        "</li>"
                        "<li>" +
                            tr("Your operating system must be installed most probably from CD ROM. To do so, open the \"CD ROM\"-section.") +
                        "</li>"
                        "<li>" +
                            tr("Your are able to choose between a physical CD drive and a CD image.") +
                        "</li>"
                        "<li>" +
                            tr("After you have choosen a CD ROM, activate \"Boot from CD ROM\".") +
                        "</li>"
                        "<li>" +
                            tr("Now you can start with the installation. "
                               "Or if you would like to start a Live-CD, you are able to do so.") +
                        "</li>"
                        "<li>" +
                            tr("A new window appears after a click on \"Start\". That is your virtual machine. "
                               "You can work inside it like you could on a real computer. "
                               "To switch keyboard and mouse between virtual and real system, press ALT+CTRL.") +
                        "</li>"
                        "<li>" +
                            tr("When you will quit the virtual system, do it like you would do it on a real system. "
                               "If the operating system does not provide a function to quit, do so with the \"Stop\" "
                               "button. But keep in mind: The stop button does the same as if you would "
                               "unplug your power cable on a real computer.") +
                        "</li>"
                        "<li>" +
                            tr("If you are sure you will kill the system, confirm it with \"Yes\".") +
                        "</li>"
                    "</ol>");
    helpText.append("<br />");

    helpText.append("<h2><a id='toolbar'>" + sectionToolbar + "</h2></a>");

    helpText.append("<ul>"
                        "<li>"
                            "<strong>" + tr("New Machine: ") + "</strong>" + tr("Click here to create a new virtual machine.") +
                        "</li>"
                        "<li>"
                            "<strong>" + tr("Open: ") + "</strong>" + tr("Click here to open an existing QtEmu machine (.qte).") +
                        "</li>"
                        "<li>"
                            "<strong>" + tr("Power buttons (Start, Stop, Restart): ") + "</strong>" + tr("These buttons do the same as the buttons inside the machine configuration. The are provided for convenience.") +
                        "</li>"
                    "</ul>");
    helpText.append("<br />");

    helpText.append("<h2><a id='options'>" + sectionOptions + "</h2></a>");

    helpText.append("<ul>"
                        "<li>"
                            "<strong>" + tr("\"MyMachines\"-Path: ") + "</strong>" + tr("You can change the default \"MyMachines\"-path, which is in the folder \"MyMachies\" in your private user / home folder. All new virtual machines will be saved by default into the \"MyMachines\" folder.") +
                        "</li>"
                        "<li>"
                            "<strong>" + tr("Tabbar position ") + "</strong>" + tr("Here you can change the position of the tabbar (default: left).") +
                        "</li>"
                        "<li>"
                            "<strong>" + tr("Language: ") + "</strong>" + tr("The language is by default your system language if available. Feel free to translate QtEmu into your language. Just contact the author.") +
                        "</li>"
                    "</ul>");
    helpText.append("<br />");

    helpText.append("<h2><a id='other'>" + sectionOther + "</h2></a>");

    helpText.append("<ul>"
                        "<li>"
                            "<strong>" + tr("Snapshot Mode: ") + "</strong>" + tr("If you would like to test something in your virtual machine without have these changes on your next restart of your virtual system, activate \"Snapshot Mode\".") +
                        "</li>"
                        "<li>"
                            "<strong>" + tr("Notes: ") + "</strong>" + tr("You are able to write down some note about the current virtual machine. Possible notes are for example applications which are installed.") +
                        "</li>"
                    "</ul>");
    helpText.append("<br />");

    m_helpTextBrowser = new QTextBrowser(this);
    m_helpTextBrowser->setReadOnly(true);
    m_helpTextBrowser->setOpenExternalLinks(false);
    m_helpTextBrowser->setHtml(helpText);

    m_closeButton = new QPushButton(QIcon::fromTheme("window-close", QIcon(QPixmap(":/images/icons/breeze/32x32/window-close.svg"))),
                                  tr("&Close"),
                                  this);

    connect(m_closeButton, &QAbstractButton::clicked,
            this, &QWidget::hide);

    QList<QKeySequence> closeShortcuts;
    closeShortcuts << QKeySequence(Qt::Key_Escape);
    closeShortcuts << QKeySequence(Qt::Key_F1);
    m_closeAction = new QAction(this);
    m_closeAction->setShortcuts(closeShortcuts);
    connect(m_closeAction, &QAction::triggered,
            this, &QWidget::hide);
    this->addAction(m_closeAction);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(2, 2, 2, 2);
    m_mainLayout->addWidget(m_helpTextBrowser);
    m_mainLayout->addWidget(m_closeButton, 0, Qt::AlignRight);
    this->setLayout(m_mainLayout);

    qDebug() << "HelpWidget created";
}

HelpWidget::~HelpWidget()
{
    qDebug() << "HelpWidget destroyed";
}

void HelpWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void HelpWidget::hideEvent(QHideEvent *event)
{
    QSettings settings;

    if (settings.isWritable()) {
        settings.setValue("HelpWidget/helpWindowSize", this->size());
    }

    event->accept();
}

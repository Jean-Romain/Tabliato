﻿/***************************************************************************
 *   copyright       : (C) 2003-2013 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "versiondialog.h"
#include "global.h"

#include <QtCore/QUrl>
#include <QDesktopServices>

VersionDialog::VersionDialog(QWidget *parent):QDialog( parent)
{
    ui.setupUi(this);
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(stopChecker()));
    ui.lineEditCurrent->setText(qApp->applicationVersion());
    ui.lineEditAvailable->setText(QString::fromUtf8("?.?.?"));
    connect(ui.pushButtonDownload, SIGNAL( clicked() ), this, SLOT( gotoDownloadPage() ) );
    connect(ui.pushButtonCheck, SIGNAL( clicked() ), this, SLOT( launchChecker() ) );
}

VersionDialog::~VersionDialog()
{
}

void VersionDialog::gotoDownloadPage()
{
    QDesktopServices::openUrl(QUrl("http://tabliato.olympe.in/download.php"));
}

void VersionDialog::launchChecker()
{
    ui.pushButtonCheck->setEnabled(false);
    timer.start(10000);
    reply = manager.get (  QNetworkRequest(QUrl("http://tabliato.olympe.in/version.txt"))  );
    QObject::connect (reply, SIGNAL (finished()),this, SLOT(showResultChecker()));

}

void VersionDialog::showResultChecker()
{
    timer.stop();

    if (reply->error())
        ui.lineEditAvailable->setText(tr("Erreur"));
    else
        ui.lineEditAvailable->setText(QString(reply->readAll()));

    ui.pushButtonCheck->setEnabled(true);
}

void VersionDialog::stopChecker()
{
    ui.lineEditAvailable->setText(tr("Erreur"));

    QObject::disconnect (reply, SIGNAL (finished()),this, SLOT(showResultChecker()));
    reply->abort();
    ui.pushButtonCheck->setEnabled(true);
}

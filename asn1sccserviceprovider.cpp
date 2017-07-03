/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QString>

#include "asn1sccserviceprovider.h"

using namespace Asn1Acn::Internal;

Q_GLOBAL_STATIC(QNetworkAccessManager, networkManagerInstance)

const QString PROGRAM_URL("/opt/asn1sccDeamon/asn1scc/Daemon/bin/Debug/Daemon.exe"); // TODO: remove hard coded path
QString BASE_URL("http://localhost:9749/"); // TODO: remove hard coded value

Asn1SccServiceProvider::Asn1SccServiceProvider()
{
    m_asn1sccService = new QProcess(this);

    m_asn1sccService->setProgram(PROGRAM_URL);
    m_asn1sccService->start();
    m_asn1sccService->waitForStarted();

    connect(m_asn1sccService, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &Asn1SccServiceProvider::onProcessFinished);
}

Asn1SccServiceProvider::~Asn1SccServiceProvider()
{
    if (m_asn1sccService->state() != QProcess::NotRunning)
        finalize();
}

QNetworkAccessManager *Asn1SccServiceProvider::getNetworkManager()
{
    return networkManagerInstance;
}

QString Asn1SccServiceProvider::getBaseURL()
{
    return BASE_URL;
}

void Asn1SccServiceProvider::finalize()
{
    networkManagerInstance->get(QNetworkRequest(QUrl(BASE_URL + "close")));
    m_asn1sccService->waitForFinished();
}

void Asn1SccServiceProvider::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);

    if (exitStatus != QProcess::NormalExit) {
        m_asn1sccService->start();
        m_asn1sccService->waitForStarted();
    }
}

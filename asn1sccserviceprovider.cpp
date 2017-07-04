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

#include "asn1sccserviceprovider.h"

#include <QString>
#include <QSettings>

#include <coreplugin/icore.h>

#include "asn1acnconstants.h"

using namespace Asn1Acn::Internal;

const int STAY_ALIVE_PERIOD_MS = 500;

Q_GLOBAL_STATIC(QNetworkAccessManager, networkManagerInstance)

Asn1SccServiceProvider::Asn1SccServiceProvider() :
    m_asn1sccService(new QProcess(this)),
    m_terminating(false)
{
    loadServiceParams();

    m_asn1sccService->setProgram(m_serviceBinaryPath);
    m_asn1sccService->setArguments(additionalArguments());

    connect(m_asn1sccService, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &Asn1SccServiceProvider::onProcessFinished);

    m_stayAliveTimer.setInterval(STAY_ALIVE_PERIOD_MS);

    connect(&m_stayAliveTimer, &QTimer::timeout,
            this, &Asn1SccServiceProvider::stayAliveTimeout, Qt::UniqueConnection);
}

Asn1SccServiceProvider::~Asn1SccServiceProvider()
{
    if (m_asn1sccService->state() != QProcess::NotRunning)
        finalize();
}

QNetworkAccessManager *Asn1SccServiceProvider::getNetworkManager() const
{
    return networkManagerInstance;
}

QString Asn1SccServiceProvider::getBaseURL() const
{
    return m_serviceBaseUrl;
}

void Asn1SccServiceProvider::start()
{
    m_asn1sccService->start();
    m_asn1sccService->waitForStarted();
    m_stayAliveTimer.start();
}

void Asn1SccServiceProvider::finalize()
{
    m_terminating = true;
    m_stayAliveTimer.stop();

    m_asn1sccService->terminate();
    m_asn1sccService->waitForFinished();
}

QStringList Asn1SccServiceProvider::additionalArguments() const
{
    QStringList arguments;

    if (!m_serviceBaseUrl.isEmpty()) {
        arguments.append(QString("--uri"));
        arguments.append(m_serviceBaseUrl);
    }

    arguments.append(QString("--watchdog"));
    arguments.append(QString::number(m_serviceStayAlivePeriod));

    return arguments;
}

void Asn1SccServiceProvider::loadServiceParams()
{
    QSettings *s = Core::ICore::settings();
    s->beginGroup(Constants::ASN1ACN_GROUP_NAME);

    m_serviceBinaryPath = s->value(Constants::ASN1ACN_SERVICE_PATH).toString();
    m_serviceBaseUrl
        = QString("http://localhost:"
                  + QString::number(s->value(Constants::ASN1ACN_SERVICE_PORT).toInt())
                  + "/");
    m_serviceStayAlivePeriod = s->value(Constants::ASN1ACN_SERVICE_WATCHDOG).toInt();

    s->endGroup();
}

void Asn1SccServiceProvider::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);

    if (!m_terminating && exitStatus != QProcess::NormalExit) {
        m_asn1sccService->start();
        m_asn1sccService->waitForStarted();
    }
}

void Asn1SccServiceProvider::stayAliveTimeout()
{
    networkManagerInstance->get(QNetworkRequest(QUrl(m_serviceBaseUrl + "stayAlive")));
}

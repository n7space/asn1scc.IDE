/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
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

#include <QJsonArray>
#include <QJsonObject>

#include <coreplugin/icore.h>

#include <utils/qtcassert.h>

#include "asn1acnconstants.h"

using namespace Asn1Acn::Internal;

Q_GLOBAL_STATIC(QNetworkAccessManager, networkManagerInstance)

Asn1SccServiceProvider::Asn1SccServiceProvider(Settings::ServiceConstPtr settings)
    : m_asn1sccService(new QProcess(this)),
      m_serviceStarted(false),
      m_settings(settings)
{
    updateConfigFromSettings();

    connect(m_settings.get(), &Settings::Service::changed, this, &Asn1SccServiceProvider::settingsChanged);

    // QProcess::finished is overloaded, hence the cast
    connect(m_asn1sccService, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &Asn1SccServiceProvider::onProcessFinished);

    connect(&m_stayAliveTimer, &QTimer::timeout,
            this, &Asn1SccServiceProvider::stayAliveTimeout, Qt::UniqueConnection);
}

Asn1SccServiceProvider::~Asn1SccServiceProvider()
{
    if (m_serviceStarted)
        stop();
}

QNetworkReply *Asn1SccServiceProvider::requestAst(const QHash<QString, DocumentSourceInfo> &documents) const
{
    QNetworkRequest astRequest(QUrl(m_settings->baseUri + "ast"));
    astRequest.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QByteArray jsonRequestData = buildAstRequestData(documents).toJson();

    return networkManagerInstance->post(astRequest, jsonRequestData);
}

void Asn1SccServiceProvider::start()
{
    QTC_ASSERT(m_serviceStarted == false, return);

    m_serviceStarted = true;

    m_stayAliveTimer.start();
    m_asn1sccService->start();
    m_asn1sccService->waitForStarted(); // TODO check for error
}

void Asn1SccServiceProvider::stop()
{
    m_serviceStarted = false;

    m_stayAliveTimer.stop();
    m_asn1sccService->terminate();
    m_asn1sccService->waitForFinished();
}

QStringList Asn1SccServiceProvider::additionalArguments() const
{
    QStringList arguments;

    if (!m_settings->baseUri.isEmpty()) {
        arguments.append(QString("--uri"));
        arguments.append(m_settings->baseUri);
    }

    arguments.append(QString("--watchdog"));
    arguments.append(QString::number(m_settings->stayAlivePeriod));

    return arguments;
}

QJsonDocument Asn1SccServiceProvider::buildAstRequestData(const QHash<QString, DocumentSourceInfo> &documents) const
{
    QJsonArray documentArray;

    QHashIterator<QString, DocumentSourceInfo> iter(documents);
    while (iter.hasNext()) {
        iter.next();

        QJsonObject asnFileData;
        asnFileData["Name"] = iter.value().getName();
        asnFileData["Contents"] = iter.value().getContent();

        documentArray.append(asnFileData);
    }

    QJsonObject files;
    files["AsnFiles"] = documentArray;
    files["AcnFiles"] = QJsonArray();

    return QJsonDocument(files);
}

void Asn1SccServiceProvider::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    if (m_serviceStarted) {
        m_asn1sccService->start();
        m_asn1sccService->waitForStarted();
    }
}

void Asn1SccServiceProvider::stayAliveTimeout()
{
    auto reply = networkManagerInstance->get(QNetworkRequest(QUrl(m_settings->baseUri + "stayAlive")));
    connect(reply, &QNetworkReply::finished, [this, reply](){
        if (m_serviceStarted && reply->error() != QNetworkReply::NoError)
            restart();
        reply->deleteLater();
    });
}

void Asn1SccServiceProvider::updateConfigFromSettings()
{
    m_asn1sccService->setProgram(m_settings->path);
    m_asn1sccService->setArguments(additionalArguments());

    m_stayAliveTimer.setInterval(m_settings->stayAlivePeriod / 2);
}

void Asn1SccServiceProvider::settingsChanged()
{
    const bool wasRunning = m_serviceStarted;

    if (wasRunning)
        stop();

    updateConfigFromSettings();

    if (wasRunning)
        start();
}

/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include <QSettings>
#include <QString>

#include <QJsonArray>
#include <QJsonObject>

#include <coreplugin/icore.h>

#include <messages/messagemanager.h>
#include <messages/messageutils.h>
#include <utils/qtcassert.h>

#include "asn1acnconstants.h"
#include "tools.h"

using namespace Asn1Acn::Internal;

Q_GLOBAL_STATIC(QNetworkAccessManager, networkManagerInstance)

Asn1SccServiceProvider::Asn1SccServiceProvider(Settings::ServiceConstPtr settings)
    : m_asn1sccService(new QProcess(this))
    , m_serviceStarted(false)
    , m_settings(settings)
{
    updateConfigFromSettings();

    connect(m_asn1sccService, &QProcess::stateChanged, [this](QProcess::ProcessState newState) {
        Q_UNUSED(newState);
        Messages::messageProcess(m_asn1sccService);
    });

    connect(m_settings.get(),
            &Settings::Service::changed,
            this,
            &Asn1SccServiceProvider::settingsChanged);

    // QProcess::finished is overloaded, hence the cast
    connect(m_asn1sccService,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,
            &Asn1SccServiceProvider::onProcessFinished);
}

Asn1SccServiceProvider::~Asn1SccServiceProvider()
{
    if (m_serviceStarted)
        stop();
}

QNetworkReply *Asn1SccServiceProvider::requestAst(const QHash<QString, QString> &documents) const
{
    QNetworkRequest astRequest(QUrl(m_settings->serviceUri() + "ast"));
    astRequest.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QByteArray jsonRequestData = buildAstRequestData(documents).toJson();

    return networkManagerInstance->post(astRequest, jsonRequestData);
}

void Asn1SccServiceProvider::start()
{
    QTC_ASSERT(m_serviceStarted == false, return );
    Messages::MessageManager::write("Starting Asn1Scc service...",
                                    Messages::MessageManager::Type::Process);
    m_serviceStarted = true;

    m_stayAliveTimer.start();
    m_asn1sccService->start();

    if (!m_asn1sccService->waitForStarted())
        Messages::messageProcessError(m_asn1sccService);
}

void Asn1SccServiceProvider::stop()
{
    Messages::MessageManager::write("Stopping Asn1Scc service...",
                                    Messages::MessageManager::Type::Process);
    m_serviceStarted = false;

    m_stayAliveTimer.stop();
    m_asn1sccService->terminate();

    if (!m_asn1sccService->waitForFinished())
        Messages::messageProcessError(m_asn1sccService);
}

QStringList Asn1SccServiceProvider::additionalArguments() const
{
    QStringList arguments;

    if (!m_settings->listeningUri().isEmpty()) {
        arguments.append(QString("--uri"));
        arguments.append(m_settings->listeningUri());
    }

    arguments.append(QString("--watchdog"));
    arguments.append(QString::number(m_settings->stayAlivePeriod()));

    return arguments;
}

namespace {
QJsonObject buildFileData(const QString &path, const QString &contents)
{
    QJsonObject asnFileData;
    asnFileData["Name"] = path;
    asnFileData["Contents"] = contents;
    return asnFileData;
}

QJsonObject buildAstGenerationOptions()
{
    QJsonObject fieldPrefix;
    fieldPrefix["Case"] = "FieldPrefixAuto";

    QJsonObject encoding;
    encoding["Case"] = "ACN";

    QJsonObject options;
    options["TypePrefix"] = "T";
    options["FieldPrefix"] = fieldPrefix;
    options["Encoding"] = encoding;

    return options;
}
} // namespace

QJsonDocument Asn1SccServiceProvider::buildAstRequestData(
    const QHash<QString, QString> &documents) const
{
    QJsonArray asnFilesArray;
    QJsonArray acnFilesArray;

    for (auto it = documents.begin(); it != documents.end(); it++) {
        if (Tools::isAsnFile(it.key()))
            asnFilesArray.append(buildFileData(it.key(), it.value()));
        else if (Tools::isAcnFile(it.key()))
            acnFilesArray.append(buildFileData(it.key(), it.value()));
    }

    QJsonObject input;
    input["AsnFiles"] = asnFilesArray;
    input["AcnFiles"] = acnFilesArray;

    /* TODO: since AST parser is not ready to work with fields introduced through ACN files,
     * it is harmful to include them in daemon call (#169)*/
    input["AcnFiles"] = QJsonArray();

    input["Options"] = buildAstGenerationOptions();

    return QJsonDocument(input);
}

void Asn1SccServiceProvider::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    Messages::MessageManager::write("Process finished...", Messages::MessageManager::Type::Process);

    if (m_serviceStarted) {
        m_asn1sccService->start();
        m_asn1sccService->waitForStarted();
    }
}

void Asn1SccServiceProvider::stayAliveTimeout()
{
    auto reply = networkManagerInstance->get(
        QNetworkRequest(QUrl(m_settings->serviceUri() + "stayAlive")));
    connect(reply,
            &QNetworkReply::finished,
            this,
            &Asn1SccServiceProvider::onStayAliveRequestFinished);
}

void Asn1SccServiceProvider::onStayAliveRequestFinished()
{
    const auto reply = qobject_cast<QNetworkReply *>(sender());
    QTC_ASSERT(reply != nullptr, return );

    if (m_serviceStarted && reply->error() != QNetworkReply::NoError) {
        Messages::messageNetworkReplyError(reply);
        restart();
    }

    reply->deleteLater();
}

void Asn1SccServiceProvider::updateConfigFromSettings()
{
    m_asn1sccService->setProgram(m_settings->path());
    m_asn1sccService->setArguments(additionalArguments());

    m_stayAliveTimer.setInterval(m_settings->stayAlivePeriod() / 2);

    if (m_settings->watchdogDisabled()) {
        disconnect(&m_stayAliveTimer,
                   &QTimer::timeout,
                   this,
                   &Asn1SccServiceProvider::stayAliveTimeout);
    } else {
        connect(&m_stayAliveTimer,
                &QTimer::timeout,
                this,
                &Asn1SccServiceProvider::stayAliveTimeout,
                Qt::UniqueConnection);
    }
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

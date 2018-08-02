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

#include "messageutils.h"

#include <QNetworkReply>
#include <QProcess>

#include "messagemanager.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Messages;

namespace {

QString networkErrorToString(QNetworkReply::NetworkError error)
{
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
        return QStringLiteral("ConnectionRefusedError");
    case QNetworkReply::RemoteHostClosedError:
        return QStringLiteral("RemoteHostClosedError");
    case QNetworkReply::HostNotFoundError:
        return QStringLiteral("HostNotFoundError");
    case QNetworkReply::TimeoutError:
        return QStringLiteral("TimeoutError");
    case QNetworkReply::OperationCanceledError:
        return QStringLiteral("OperationCanceledError");
    case QNetworkReply::SslHandshakeFailedError:
        return QStringLiteral("SslHandshakeFailedError");
    case QNetworkReply::TemporaryNetworkFailureError:
        return QStringLiteral("TemporaryNetworkFailureError");
    case QNetworkReply::NetworkSessionFailedError:
        return QStringLiteral("NetworkSessionFailedError");
    case QNetworkReply::BackgroundRequestNotAllowedError:
        return QStringLiteral("BackgroundRequestNotAllowedError");
    case QNetworkReply::TooManyRedirectsError:
        return QStringLiteral("TooManyRedirectsError");
    case QNetworkReply::InsecureRedirectError:
        return QStringLiteral("InsecureRedirectError");
    case QNetworkReply::UnknownNetworkError:
        return QStringLiteral("UnknownNetworkError");

    case QNetworkReply::ProxyConnectionRefusedError:
        return QStringLiteral("ProxyConnectionRefusedError");
    case QNetworkReply::ProxyConnectionClosedError:
        return QStringLiteral("ProxyConnectionClosedError");
    case QNetworkReply::ProxyNotFoundError:
        return QStringLiteral("ProxyNotFoundError");
    case QNetworkReply::ProxyTimeoutError:
        return QStringLiteral("ProxyTimeoutError");
    case QNetworkReply::ProxyAuthenticationRequiredError:
        return QStringLiteral("ProxyAuthenticationRequiredError");
    case QNetworkReply::UnknownProxyError:
        return QStringLiteral("UnknownProxyError");

    case QNetworkReply::ContentAccessDenied:
        return QStringLiteral("ContentAccessDenied");
    case QNetworkReply::ContentOperationNotPermittedError:
        return QStringLiteral("ContentOperationNotPermittedError");
    case QNetworkReply::ContentNotFoundError:
        return QStringLiteral("ContentNotFoundError");
    case QNetworkReply::AuthenticationRequiredError:
        return QStringLiteral("AuthenticationRequiredError");
    case QNetworkReply::ContentReSendError:
        return QStringLiteral("ContentReSendError");
    case QNetworkReply::ContentConflictError:
        return QStringLiteral("ContentConflictError");
    case QNetworkReply::ContentGoneError:
        return QStringLiteral("ContentGoneError");
    case QNetworkReply::UnknownContentError:
        return QStringLiteral("UnknownContentError");

    case QNetworkReply::ProtocolUnknownError:
        return QStringLiteral("ProtocolUnknownError");
    case QNetworkReply::ProtocolInvalidOperationError:
        return QStringLiteral("ProtocolInvalidOperationError");
    case QNetworkReply::ProtocolFailure:
        return QStringLiteral("ProtocolFailure");

    case QNetworkReply::InternalServerError:
        return QStringLiteral("InternalServerError");
    case QNetworkReply::OperationNotImplementedError:
        return QStringLiteral("OperationNotImplementedError");
    case QNetworkReply::ServiceUnavailableError:
        return QStringLiteral("ServiceUnavailableError");
    case QNetworkReply::UnknownServerError:
        return QStringLiteral("UnknownServerError");
    default:
        return {};
    }
}

QString networkReplyError(const QNetworkReply *reply)
{
    return "Description: " + networkErrorToString(reply->error());
}

QString networkReplyStatus(const QNetworkReply *reply)
{
    return reply->error() == QNetworkReply::NoError ? QString("Status: Success.")
                                                    : QString("Status: Error.");
}

QString networkReplyUrl(const QNetworkReply *reply)
{
    return reply->request().url().toString();
}

QString networkReplyBaseMessage(const QNetworkReply *reply)
{
    return networkReplyUrl(reply) + ": " + networkReplyStatus(reply);
}

QString processStateToString(const QProcess::ProcessState state)
{
    switch (state) {
    case QProcess::NotRunning:
        return QStringLiteral("NotRunning");
    case QProcess::Starting:
        return QStringLiteral("Starting");
    case QProcess::Running:
        return QStringLiteral("Running");
    default:
        return {};
    }
}

QString processState(const QProcess *process)
{
    return "State: " + processStateToString(process->state()) + ".";
}

QString processErrorToString(const QProcess::ProcessError error)
{
    switch (error) {
    case QProcess::FailedToStart:
        return QStringLiteral("FailedToStart");
    case QProcess::Crashed:
        return QStringLiteral("Crashed");
    case QProcess::Timedout:
        return QStringLiteral("Timedout");
    case QProcess::ReadError:
        return QStringLiteral("ReadError");
    case QProcess::WriteError:
        return QStringLiteral("WriteError");
    case QProcess::UnknownError:
        return QStringLiteral("Error");
    default:
        return {};
    }
}

QString processError(const QProcess *process)
{
    return "Description: " + processErrorToString(process->error());
}

QString processBaseMessage(const QProcess *process)
{
    return process->program() + ": " + processState(process);
}

} // namespace

void Asn1Acn::Internal::Messages::messageNetworkReply(const QNetworkReply *reply)
{
    MessageManager::write(networkReplyBaseMessage(reply), MessageManager::Type::Communication);
}

void Asn1Acn::Internal::Messages::messageNetworkReplyError(const QNetworkReply *reply)
{
    QString message = networkReplyBaseMessage(reply) + " " + networkReplyError(reply);
    MessageManager::write(message, MessageManager::Type::Communication);
}

void Asn1Acn::Internal::Messages::messageProcess(const QProcess *process)
{
    MessageManager::write(processBaseMessage(process), MessageManager::Type::Process);
}

void Asn1Acn::Internal::Messages::messageProcessError(const QProcess *process)
{
    QString message = processBaseMessage(process) + " " + processError(process);
    MessageManager::write(message, MessageManager::Type::Process);
}

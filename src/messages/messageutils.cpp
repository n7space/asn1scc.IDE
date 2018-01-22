/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
    switch(error)
    {
    case QNetworkReply::ConnectionRefusedError:
        return "ConnectionRefusedError";
    case QNetworkReply::RemoteHostClosedError:
        return "RemoteHostClosedError";
    case QNetworkReply::HostNotFoundError:
        return "HostNotFoundError";
    case QNetworkReply::TimeoutError:
        return "TimeoutError";
    case QNetworkReply::OperationCanceledError:
        return "OperationCanceledError";
    case QNetworkReply::SslHandshakeFailedError:
        return "SslHandshakeFailedError";
    case QNetworkReply::TemporaryNetworkFailureError:
        return "TemporaryNetworkFailureError";
    case QNetworkReply::NetworkSessionFailedError:
        return "NetworkSessionFailedError";
    case QNetworkReply::BackgroundRequestNotAllowedError:
        return "BackgroundRequestNotAllowedError";
    case QNetworkReply::TooManyRedirectsError:
        return "TooManyRedirectsError";
    case QNetworkReply::InsecureRedirectError:
        return "InsecureRedirectError";
    case QNetworkReply::UnknownNetworkError:
        return "UnknownNetworkError";

    case QNetworkReply::ProxyConnectionRefusedError:
        return "ProxyConnectionRefusedError";
    case QNetworkReply::ProxyConnectionClosedError:
        return "ProxyConnectionClosedError";
    case QNetworkReply::ProxyNotFoundError:
        return "ProxyNotFoundError";
    case QNetworkReply::ProxyTimeoutError:
        return "ProxyTimeoutError";
    case QNetworkReply::ProxyAuthenticationRequiredError:
        return "ProxyAuthenticationRequiredError";
    case QNetworkReply::UnknownProxyError:
        return "UnknownProxyError";

    case QNetworkReply::ContentAccessDenied:
        return "ContentAccessDenied";
    case QNetworkReply::ContentOperationNotPermittedError:
        return "ContentOperationNotPermittedError";
    case QNetworkReply::ContentNotFoundError:
        return "ContentNotFoundError";
    case QNetworkReply::AuthenticationRequiredError:
        return "AuthenticationRequiredError";
    case QNetworkReply::ContentReSendError:
        return "ContentReSendError";
    case QNetworkReply::ContentConflictError:
        return "ContentConflictError";
    case QNetworkReply::ContentGoneError:
        return "ContentGoneError";
    case QNetworkReply::UnknownContentError:
        return "UnknownContentError";

    case QNetworkReply::ProtocolUnknownError:
        return "ProtocolUnknownError";
    case QNetworkReply::ProtocolInvalidOperationError:
        return "ProtocolInvalidOperationError";
    case QNetworkReply::ProtocolFailure:
        return "ProtocolFailure";

    case QNetworkReply::InternalServerError:
        return "InternalServerError";
    case QNetworkReply::OperationNotImplementedError:
        return "OperationNotImplementedError";
    case QNetworkReply::ServiceUnavailableError:
        return "ServiceUnavailableError";
    case QNetworkReply::UnknownServerError:
        return "UnknownServerError";
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
    return reply->error() == QNetworkReply::NoError
            ? QString("Status: Success.") : QString("Status: Error.");
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
        return "NotRunning";
    case QProcess::Starting:
        return "Starting";
    case QProcess::Running:
        return "Running";
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
        return "FailedToStart";
    case QProcess::Crashed:
        return "Crashed";
    case QProcess::Timedout:
        return "Timedout";
    case QProcess::ReadError:
        return "ReadError";
    case QProcess::WriteError:
        return "WriteError";
    case QProcess::UnknownError:
        return "Error";
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

} // namespace Anonymous

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

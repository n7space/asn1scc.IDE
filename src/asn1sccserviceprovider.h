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

#pragma once

#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QJsonDocument>

#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QHash>

#include "settings/service.h"

#include "parsingserviceprovider.h"

namespace Asn1Acn {
namespace Internal {

class Asn1SccServiceProvider
        : public ParsingServiceProvider
{
    Q_OBJECT

public:
    Asn1SccServiceProvider(Settings::ServiceConstPtr settings);
    ~Asn1SccServiceProvider();

    QNetworkReply *requestAst(const QHash<QString, QString> &documents) const override;

    void start();
    void stop();
    void restart() { stop(); start(); }

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void stayAliveTimeout();
    void settingsChanged();
    void onStayAliveRequestFinished();

private:
    void updateConfigFromSettings();
    QStringList additionalArguments() const;

    QJsonDocument buildAstRequestData(const QHash<QString, QString> &documents) const;

    QProcess *m_asn1sccService;
    QTimer m_stayAliveTimer;
    bool m_serviceStarted;

    Settings::ServiceConstPtr m_settings;
};

} /* namespace Asn1Acn */
} /* namespace Internal */

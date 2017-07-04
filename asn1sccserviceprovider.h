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

#pragma once

#include <QNetworkAccessManager>

#include <QObject>
#include <QProcess>
#include <QTimer>

namespace Asn1Acn {
namespace Internal {

class Asn1SccServiceProvider : public QObject
{
    Q_OBJECT

public:
    Asn1SccServiceProvider();
    ~Asn1SccServiceProvider();

    QNetworkAccessManager *getNetworkManager() const;
    QString getBaseURL() const;

    void start();
    void finalize();

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void stayAliveTimeout();

private:
    QStringList additionalArguments() const;
    void loadServiceParams();

    QProcess *m_asn1sccService;
    QTimer m_stayAliveTimer;
    bool m_terminating;

    QString m_serviceBaseUrl;
    QString m_serviceBinaryPath;
    int m_serviceStayAlivePeriod;
};

} /* namespace Asn1Acn */
} /* namespace Internal */

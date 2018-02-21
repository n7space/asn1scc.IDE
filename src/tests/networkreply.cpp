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

#include <QTimer>

#include "networkreply.h"

using namespace Asn1Acn::Internal::Tests;

static const int REPLY_WAIT_TIME_MS = 100;

NetworkReply::NetworkReply()
{
    open(QIODevice::ReadWrite);
}

NetworkReply::~NetworkReply()
{
    close();
}

void NetworkReply::setErrored()
{
    setError(QNetworkReply::NetworkError::ConnectionRefusedError, "Errored");
}

void NetworkReply::run()
{
    QTimer *timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &NetworkReply::onTimerTimeout);

    timer->setSingleShot(true);
    timer->setInterval(REPLY_WAIT_TIME_MS);
    timer->start();
}

void NetworkReply::onTimerTimeout()
{
    QTimer *timer = qobject_cast<QTimer *>(sender());
    timer->deleteLater();

    emit finished();
}

qint64 NetworkReply::readData(char *data, qint64 maxlen)
{
    if (m_data.size() == 0)
        return -1;

    qint64 storedLen = m_data.size();
    qint64 len = maxlen > storedLen ? storedLen : maxlen;
    memcpy(data, m_data.data(), len);

    std::vector<char>(m_data.begin() + len, m_data.end()).swap(m_data);

    return len;
}

qint64 NetworkReply::writeData(const char *data, qint64 len)
{
    m_data.resize(len);
    memcpy(m_data.data(), data, len);
    return len;
}

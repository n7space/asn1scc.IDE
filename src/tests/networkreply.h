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

#pragma once

#include <vector>

#include <QNetworkReply>

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class NetworkReply : public QNetworkReply
{
public:
    NetworkReply();
    ~NetworkReply();

    void run();
    void setErrored();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    void abort() override {}

private:
    void onTimerTimeout();

    std::vector<char> m_data;
    // qint64 m_len;
};

} /* namespace Tests */
} // namespace Internal
} // namespace Asn1Acn

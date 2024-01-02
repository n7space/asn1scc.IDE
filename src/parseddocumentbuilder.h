/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <memory>
#include <vector>

#include <QObject>
#include <QString>

#include <data/errormessage.h>
#include <data/file.h>

namespace Asn1Acn {
namespace Internal {

class ParsedDocumentBuilder : public QObject
{
    Q_OBJECT

public:
    virtual ~ParsedDocumentBuilder() = default;

    virtual void run() = 0;

    virtual std::vector<std::unique_ptr<Data::File>> takeDocuments() = 0;
    virtual const std::vector<Data::ErrorMessage> &errorMessages() const = 0;

signals:
    void finished();
    void errored();
    void failed();
};

} /* namespace Internal */
} /* namespace Asn1Acn */

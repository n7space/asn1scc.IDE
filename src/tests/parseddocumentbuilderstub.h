/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#include <memory>

#include <QHash>
#include <QString>

#include "data/file.h"

#include "parseddocumentbuilder.h"

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class ParsedDocumentBuilderStub : public ParsedDocumentBuilder
{
    Q_OBJECT

public:
    ParsedDocumentBuilderStub(const QHash<QString, QString> &documents);

    void run() override;

    std::vector<std::unique_ptr<Data::File>> takeDocuments() override;
    const std::vector<Data::ErrorMessage> &errorMessages() const override;

private:
    const QHash<QString, QString> &m_rawDocuments;
    std::vector<std::unique_ptr<Data::File>> m_parsedDocuments;

    std::vector<Data::ErrorMessage> m_errorMessages;
};

} /* namespace Tests */
} /* namespace Internal */
} /* namespace Asn1Acn */

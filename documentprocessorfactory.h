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

#pragma once

#include "documentprocessor.h"
#include "documentprocessorstub.h"
#include "asn1sccdocumentprocessor.h"

namespace Asn1Acn {
namespace Internal {

class DocumentProcessorFactory
{
public:
    enum class WorkMode {
        Test,
        Real
    };

    DocumentProcessorFactory(WorkMode mode = WorkMode::Real) : m_mode(mode) {}

    DocumentProcessor *create(const QString &projectName) const {
        switch(m_mode) {
            case WorkMode::Test:
                return new DocumentProcessorStub();
            case WorkMode::Real:
                return Asn1SccDocumentProcessor::create(projectName);
            default:
                return nullptr;
        }
    }

private:
    WorkMode m_mode;
};

} // namespace Internal
} // namespace Asn1Acn

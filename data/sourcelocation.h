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

#include <QString>

namespace Asn1Acn {
namespace Internal {
namespace Data {

class SourceLocation
{
public:
    SourceLocation()
        : m_line(-1), m_column(-1)
    {}

    SourceLocation(const QString &path, int line, int column)
        : m_path(path), m_line(line), m_column(column)
    {}

    const QString fileName() const;
    const QString &path() const { return m_path; }
    int line() const { return m_line; }
    int column() const { return m_column; }

    bool isValid() const { return !m_path.isEmpty() && m_line != -1 && m_column != -1; }

private:
    QString m_path;
    int m_line;
    int m_column;
};

bool operator==(const SourceLocation &a, const SourceLocation &b);
bool operator!=(const SourceLocation &a, const SourceLocation &b);

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

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
#include <QStringList>
#include <QList>

#include "import.h"

#include "librarynode.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class Element : public LibraryNode
{
public:
    Element(const QString &name, const QString &description)
        : LibraryNode(name, description)
    {}

    const QStringList &asn1Files() const { return m_asn1Files; }
    const QStringList &conflicts() const { return m_conflicts; }
    const QStringList &requirements() const { return m_requirements; }
    const QList<Import> &imports() const { return m_imports; }

    void addAsn1File(const QString &file) { m_asn1Files.append(file); }
    void addConflict(const QString &conflict) { m_conflicts.append(conflict); }
    void addRequirement(const QString &requirement) { m_requirements.append(requirement); }
    void addImport(const Import &import) { m_imports.append(import); }

    LibraryNode *child(int num) const override { Q_UNUSED(num); return nullptr; }
    int childrenCount() const override { return 0; }
    int childIndex(const LibraryNode *child) const override { Q_UNUSED(child); return -1; }

private:
    QStringList m_asn1Files;
    QStringList m_conflicts;
    QStringList m_requirements;
    QList<Import> m_imports;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

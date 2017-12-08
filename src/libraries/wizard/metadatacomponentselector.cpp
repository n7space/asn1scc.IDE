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

#include "metadatacomponentselector.h"

#include <QDirIterator>

#include <libraries/librarystorage.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

MetadaComponentSelector::MetadaComponentSelector(MetadataModel *model, const QString &path, QObject *parent)
    : ComponentSelector(parent)
    , m_model(model)
    , m_path(path)
{
}

QStringList MetadaComponentSelector::pathsFromNames(const QStringList &names)
{
    if (names.empty())
        return {};

    QDirIterator it(m_path, names, QDir::NoFilter, QDirIterator::Subdirectories);

    QStringList paths;
    while (it.hasNext())
        paths.append(it.next());

    return paths;
}

namespace {

QStringList readElement(const Metadata::Element *element)
{
    if (element->checked() != Qt::Checked)
        return {};

    return element->asn1Files();
}

QStringList readSubmodule(const Metadata::Submodule *submodule)
{
    QStringList files;
    for (const auto &element : submodule->elements())
        files.append(readElement(element.get()));

    return files;
}

QStringList readModule(const Metadata::Module *module)
{
    QStringList files;
    for (const auto &submodule : module->submodules())
        files.append(readSubmodule(submodule.get()));

    return files;
}

} // namespace Anonymous

QStringList MetadaComponentSelector::pathsToImport()
{
    const auto lib = LibraryStorage::instance()->library(m_path);

    if (lib == nullptr)
        return {};

    QStringList files;
    for (const auto &module : lib->modules())
        files.append(readModule(module.get()));

    files.removeDuplicates();

    return pathsFromNames(files);
}

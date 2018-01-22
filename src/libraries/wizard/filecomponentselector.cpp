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

#include "filecomponentselector.h"

#include <QDirIterator>

#include <utils/qtcassert.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

FileComponentSelector::FileComponentSelector(FileModel *model, const QStringList nameFilter, QObject *parent)
    : ComponentSelector(parent)
    , m_model(model)
    , m_nameFilter(nameFilter)
{
}

QStringList FileComponentSelector::pathsToImport()
{
    return selectedPathsFromModelItem(m_model->rootPath());
}

QStringList FileComponentSelector::selectedPathsFromModelItem(const QString &parentPath) const
{
    const auto parent = m_model->index(parentPath);

    if (!parent.isValid())
        return {};

    QStringList paths;
    for (int i = 0; i < m_model->rowCount(parent); ++i)
        for (int j = 0; j < m_model->columnCount(parent); ++j)
            paths.append(pathsFromChildIndex(parent.child(i, j)));

    return paths;
}

QStringList FileComponentSelector::pathsFromChildIndex(const QModelIndex &child) const
{
    const auto state = child.data(Qt::CheckStateRole);
    if (state == Qt::Unchecked)
        return {};

    if (m_model->isDir(child))
        return m_model->rowCount(child) > 0
               ? selectedPathsFromModelItem(m_model->filePath(child))
               : selectedPathsFromFilesystemItem(m_model->filePath(child));

    QTC_ASSERT(state == Qt::Checked, return {});

    return { m_model->filePath(child) };
}

QStringList FileComponentSelector::selectedPathsFromFilesystemItem(const QString &path) const
{
    QDirIterator it(path, m_nameFilter, QDir::Files, QDirIterator::Subdirectories);

    QStringList paths;
    while (it.hasNext())
        paths.append(it.next());

    return paths;
}

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

#include "metadatacomponentselector.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QMessageBox>

#include <libraries/librarystorage.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

MetadaComponentSelector::MetadaComponentSelector(MetadataModel *model,
                                                 const QString &path,
                                                 QObject *parent)
    : ComponentSelector(parent)
    , m_model(model)
    , m_path(path)
{
    connect(m_model,
            &MetadataModel::conflictOccurred,
            this,
            &MetadaComponentSelector::onConflictOccured);
}

void MetadaComponentSelector::onConflictOccured(const QString &first, const QString &second) const
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Conflict detected");
    msgBox.setText("\"" + first + "\"" + " and " + "\"" + second + "\"" + " conflicts");
    msgBox.exec();
}

Utils::FilePaths MetadaComponentSelector::pathsToImport()
{
    auto files = fileNamesFromSelectedItems();

    files.removeDuplicates();

    return insertAcnFiles(pathsFromNames(files));
}

QStringList MetadaComponentSelector::fileNamesFromSelectedItems() const
{
    QStringList fileNames;
    const auto &selectedItems = m_model->selectedItems();
    for (auto it = selectedItems.begin(); it != selectedItems.end(); ++it) {
        if (it.value() != Qt::Checked)
            continue;

        const auto libraryNode = m_model->dataNode(it.key());
        fileNames.append(libraryNode->asn1Files());
        fileNames.append(libraryNode->additionalFiles());
    }

    return fileNames;
}

Utils::FilePaths MetadaComponentSelector::insertAcnFiles(const Utils::FilePaths &asnFiles) const
{
    Utils::FilePaths allFiles = asnFiles;

    static const QRegularExpression re("\\.asn1?$");

    for (auto file : asnFiles) {
        QString filePath = file.path();
        const auto match = re.match(filePath);
        if (!match.hasMatch())
            continue;

        filePath.replace(match.capturedStart(), match.capturedLength(), ".acn");

        if (QFileInfo::exists(filePath))
            allFiles << Utils::FilePath::fromString(filePath);
    }

    return allFiles;
}

Utils::FilePaths MetadaComponentSelector::pathsFromNames(const QStringList &names) const
{
    if (names.empty())
        return {};

    QDirIterator it(m_path, names, QDir::NoFilter, QDirIterator::Subdirectories);

    Utils::FilePaths paths;
    while (it.hasNext())
        paths.append(Utils::FilePath::fromString(it.next()));

    return paths;
}

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

#include "tools.h"

#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QStringList>

#include <coreplugin/editormanager/editormanager.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>

#include "asn1acnconstants.h"

using namespace Asn1Acn;

static QStringList matchingSuffixesForFile(const QString &path)
{
    auto mimeType = Utils::mimeTypeForFile(path).name();
    if (mimeType == Constants::ASN1_MIMETYPE)
        return Utils::mimeTypeForName(Constants::ACN_MIMETYPE).suffixes();
    if (mimeType == Constants::ACN_MIMETYPE)
        return Utils::mimeTypeForName(Constants::ASN1_MIMETYPE).suffixes();
    return QStringList();
}

static QStringList baseNameWithAllSuffixes(const QString &baseName, const QStringList &suffixes)
{
    QStringList result;
    const QChar dot = QLatin1Char('.');
    foreach (const QString &suffix, suffixes) {
        QString fileName = baseName;
        fileName += dot;
        fileName += suffix;
        result += fileName;
    }
    return result;
}

static QString correspondingDataOrEncoding(const QString &path)
{
    const QFileInfo fileInfo(path);
    const auto suffixes = matchingSuffixesForFile(path);
    const auto candidateFiles = baseNameWithAllSuffixes(fileInfo.completeBaseName(), suffixes);

    foreach (const auto fileName, candidateFiles) {
        const QFileInfo candidateFileInfo(fileInfo.absoluteDir(), fileName);
        if (candidateFileInfo.isFile())
            return candidateFileInfo.absoluteFilePath();
    }

    return QString::null;
}

static QString findDataOrEncodingForCurrentDocument()
{
    const Core::IDocument *currentDocument = Core::EditorManager::currentDocument();
    QTC_ASSERT(currentDocument, return QString::null);
    return correspondingDataOrEncoding(currentDocument->filePath().toString());
}

void Asn1Acn::Internal::Tools::switchBetweenDataAndEncoding()
{
    const QString otherFile = findDataOrEncodingForCurrentDocument();
    if (!otherFile.isEmpty())
        Core::EditorManager::openEditor(otherFile);
}

void Asn1Acn::Internal::Tools::switchBetweenDataAndEncodingInNextSplit()
{
    const QString otherFile = findDataOrEncodingForCurrentDocument();
    if (!otherFile.isEmpty())
        Core::EditorManager::openEditor(otherFile,
                                        Core::Id(),
                                        Core::EditorManager::OpenInOtherSplit);
}

bool Asn1Acn::Internal::Tools::isAsnFile(const QString &path)
{
    return Utils::mimeTypeForFile(path).name() == Constants::ASN1_MIMETYPE;
}

bool Asn1Acn::Internal::Tools::isAcnFile(const QString &path)
{
    return Utils::mimeTypeForFile(path).name() == Constants::ACN_MIMETYPE;
}

/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include "componentlibrarydispatcher.h"

#include <QRegularExpression>

#include "componentlibraryprocessor.h"
#include "generalmetadataprocessor.h"

#include "filesourcereader.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;

namespace {
QStringList filterByDirectory(const QStringList &files, const QString &directory)
{
    static const QString slashOrBackslash = QLatin1String("(") + QRegularExpression::escape("/")
                                            + QLatin1String("|") + QRegularExpression::escape("\\")
                                            + QLatin1String(")");

    QRegularExpression re(QRegularExpression::escape(directory) + slashOrBackslash);

    return files.filter(re);
}

void tryStartGeneralMetadataProcessing(const QStringList &filesInDirectory,
                                       const QString &directory,
                                       const FileSourceReader &reader)
{
    const auto generalProcessor
        = new GeneralMetadataProcessor(directory,
                                       filesInDirectory.filter("info.json").join(" "),
                                       reader);
    generalProcessor->process();
}

void tryStartServicesMetadataProcessing(const QStringList &filesInDirectory,
                                        const QString &directory,
                                        const FileSourceReader &reader)
{
    const auto metaFiles = filesInDirectory.filter("meta.json");
    if (metaFiles.empty())
        return;

    const auto componentProcessor = new ComponentLibraryProcessor(directory, metaFiles, reader);
    componentProcessor->process();
}
} // namespace

void CompontentLibraryDispatcher::dispatch(const QStringList &directories, const QStringList &files)
{
    static const FileSourceReader reader;

    for (const auto &directory : directories) {
        const auto filesInDirectory = filterByDirectory(files, directory);

        tryStartGeneralMetadataProcessing(filesInDirectory, directory, reader);
        tryStartServicesMetadataProcessing(filesInDirectory, directory, reader);
    }
}

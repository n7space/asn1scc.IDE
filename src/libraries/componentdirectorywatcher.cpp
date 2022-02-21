/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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

#include "componentdirectorywatcher.h"

#include <QDir>

#include "librarystorage.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;

static const int WAIT_TIMEOUT_MS = 2000;

ComponentDirectoryWatcher::ComponentDirectoryWatcher(Settings::LibrariesConstPtr libraries,
                                                     QObject *parent)
    : QObject(parent)
    , m_fsWatcher(std::make_unique<QFileSystemWatcher>())
    , m_libraries(libraries)
{
    resetWatched();

    m_resetWatch.setInterval(WAIT_TIMEOUT_MS);
    m_resetWatch.setSingleShot(true);

    connect(&m_resetWatch,
            &QTimer::timeout,
            this,
            &ComponentDirectoryWatcher::resetWatched,
            Qt::UniqueConnection);

    connect(m_fsWatcher.get(),
            &QFileSystemWatcher::fileChanged,
            this,
            &ComponentDirectoryWatcher::filesChanged,
            Qt::UniqueConnection);
    connect(m_fsWatcher.get(),
            &QFileSystemWatcher::directoryChanged,
            this,
            &ComponentDirectoryWatcher::filesChanged,
            Qt::UniqueConnection);
    connect(m_libraries.get(),
            &Settings::Libraries::changed,
            this,
            &ComponentDirectoryWatcher::configChanged,
            Qt::UniqueConnection);
}

void ComponentDirectoryWatcher::configChanged()
{
    m_resetWatch.start();
}

void ComponentDirectoryWatcher::filesChanged(const QString &path)
{
    Q_UNUSED(path);
    m_resetWatch.start();
}

void ComponentDirectoryWatcher::resetWatched()
{
    removeAll();
    addAllLibraries();
}

void ComponentDirectoryWatcher::addAllLibraries()
{
    const auto libPaths = m_libraries->libPaths();
    for (const auto &libPath : libPaths)
        addMainDirectory(libPath);

    CompontentLibraryDispatcher::dispatch(libPaths, m_fsWatcher->files());
}

void ComponentDirectoryWatcher::addMainDirectory(const QString &path)
{
    const QDir dir(path);
    if (!dir.exists())
        return;

    m_fsWatcher->addPath(path);

    if (dir.exists(QStringLiteral("info.json")))
        m_fsWatcher->addPath(dir.filePath(QStringLiteral("info.json")));

    const auto subDirectories = dir.entryList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
    for (const auto &subPath : subDirectories)
        addSubDirectory(dir.filePath(subPath));
}

void ComponentDirectoryWatcher::addSubDirectory(const QString &path)
{
    const QDir dir(path);
    if (!dir.exists())
        return;

    m_fsWatcher->addPath(path);

    if (dir.exists(QStringLiteral("meta.json")))
        m_fsWatcher->addPath(dir.filePath(QStringLiteral("meta.json")));
}

void ComponentDirectoryWatcher::removeAll()
{
    const auto directories = m_fsWatcher->directories();
    if (!directories.empty())
        m_fsWatcher->removePaths(directories);

    const auto files = m_fsWatcher->files();
    if (!files.empty())
        m_fsWatcher->removePaths(files);

    auto storage = LibraryStorage::instance();

    storage->removeLibraries();
    storage->removeAllMetadata();
}

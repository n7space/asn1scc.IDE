/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
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

#include <QFuture>
#include <QObject>
#include <QPointer>
#include <QString>

namespace Core {
class SearchResult;
class SearchResultItem;
} // namespace Core

namespace Asn1Acn {
namespace Internal {

namespace Data {
class TypeReference;
class SourceLocation;
} // namespace Data

class ParsedDataStorage;
class SourceReader;

struct UsagesFinderParameters
{
    QString module;
    QString type;
};

class UsagesFinder : public QObject
{
    Q_OBJECT
public:
    explicit UsagesFinder(ParsedDataStorage *storage,
                          std::unique_ptr<SourceReader> reader,
                          QObject *parent = nullptr);
    ~UsagesFinder() override;

    void findUsages(const QString &module, const QString &type);

private:
    void searchAgain();
    void findAll(Core::SearchResult *search, const UsagesFinderParameters &params);
    void createWatcher(const QFuture<Data::TypeReference> &future, Core::SearchResult *search);
    QString readLine(const Data::SourceLocation &loc);
    void displayResults(Core::SearchResult *search,
                        QFutureWatcher<Data::TypeReference> *watcher,
                        int first,
                        int last);

    QPointer<ParsedDataStorage> m_storage;
    std::unique_ptr<SourceReader> m_reader;
};

} // namespace Internal
} // namespace Asn1Acn

Q_DECLARE_METATYPE(Asn1Acn::Internal::UsagesFinderParameters)

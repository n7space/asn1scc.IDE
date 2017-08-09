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

#include "indenter.h"

#include <texteditor/tabsettings.h>

using namespace Asn1Acn::Internal;

Indenter::Indenter()
{
}

Indenter::~Indenter()
{
}

bool Indenter::isElectricCharacter(const QChar &ch) const
{
    return ch == QLatin1Char('{') || ch == QLatin1Char('}');
}

static int adjustForClosingBracket(const QTextBlock &block)
{
    QString currentText = block.text();
    const auto closingCount = currentText.count(QLatin1Char('}'));
    if (closingCount <= 0)
        return 0;
    const auto openingCount = currentText.count(QLatin1Char('{'));
    if (closingCount > openingCount)
        return closingCount - openingCount;
    return 0;
}

int Indenter::indentFor(const QTextBlock &block,
                        const TextEditor::TabSettings &tabSettings)
{
    QTextBlock previous = block.previous();
    if (!previous.isValid())
        return 0;

    QString previousText = previous.text();
    while (previousText.trimmed().isEmpty()) {
        previous = previous.previous();
        if (!previous.isValid())
            return 0;
        previousText = previous.text();
    }

    int indent = tabSettings.indentationColumn(previousText);

    int adjust = previousText.count(QLatin1Char('{')) - previousText.count(QLatin1Char('}'));
    adjust -= adjustForClosingBracket(block);
    adjust *= tabSettings.m_indentSize;

    return qMax(0, indent + adjust);
}

/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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
#include "selectionpositionresolver.h"

#include <QTextCursor>

using namespace Asn1Acn::Internal;

void SelectionPositionResolver::resolve(const QTextCursor &cursor)
{
    m_selectionStartPosition = m_selectionEndPosition = cursor.position();

    QTextCursor moved = cursor;
    initSelection(moved);
    if (!moved.hasSelection())
        return;

    m_selectionStartPosition = leftSelectionBound(moved);
    m_selectionEndPosition = rightSelectionBound(moved);
}

void SelectionPositionResolver::initSelection(QTextCursor &cursor) const
{
    cursor.select(QTextCursor::WordUnderCursor);
}

static QChar proceedPosition(QTextCursor &cursor, QTextCursor::MoveOperation direction)
{
    cursor.clearSelection();
    cursor.movePosition(direction, QTextCursor::KeepAnchor);

    const auto text = cursor.selectedText();
    return text.size() > 0 ? text.at(0) : QChar();
}

static int selectionBound(const QTextCursor &cursor,
                          QTextCursor::MoveOperation direction,
                          const int initPos)
{
    QTextCursor moved = cursor;
    moved.setPosition(initPos);

    int pos;
    QChar selected;

    do {
        pos = moved.position();
        selected = proceedPosition(moved, direction);
    } while (selected.isLetterOrNumber() || selected == QLatin1Char('-'));

    return pos;
}

int SelectionPositionResolver::leftSelectionBound(const QTextCursor &cursor) const
{
    return selectionBound(cursor, QTextCursor::Left, cursor.selectionStart());
}

int SelectionPositionResolver::rightSelectionBound(const QTextCursor &cursor) const
{
    return selectionBound(cursor, QTextCursor::Right, cursor.selectionEnd());
}

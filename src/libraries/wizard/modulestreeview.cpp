/****************************************************************************
**
** Copyright (C) 2018-2024 N7 Space sp. z o. o.
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

// This class was created to overcome issue with tree items being involuntary checked.
// For more information about the issue see QTBUG-59067.

#include "modulestreeview.h"

using namespace Asn1Acn::Internal::Libraries::Wizard;

ModulesTreeView::ModulesTreeView(QWidget *parent)
    : QTreeView(parent)
    , m_blockEvents(false)
{
    connect(this, &QTreeView::collapsed, this, &ModulesTreeView::onExpansionChanged);

    connect(this, &QTreeView::expanded, this, &ModulesTreeView::onExpansionChanged);
}

void ModulesTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_blockEvents) {
        m_blockEvents = false;
        return;
    }

    QTreeView::mouseReleaseEvent(event);
}

void ModulesTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_blockEvents)
        return;

    QTreeView::mouseMoveEvent(event);
}

void ModulesTreeView::onExpansionChanged()
{
    m_blockEvents = true;
}

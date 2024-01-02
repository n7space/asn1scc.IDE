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
#pragma once

#include <QTreeView>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Wizard {

class ModulesTreeView : public QTreeView
{
    Q_OBJECT
public:
    ModulesTreeView(QWidget *parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void onExpansionChanged();

private:
    bool m_blockEvents;
};

} // namespace Wizard
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

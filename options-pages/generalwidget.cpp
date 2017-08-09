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
#include "generalwidget.h"

using namespace Asn1Acn::Internal::OptionsPages;

GeneralWidget::GeneralWidget(QWidget *parent) : QWidget(parent)
{
    m_ui.setupUi(this);

    m_ui.asn1sccPathChooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
}

QString GeneralWidget::asn1sccPath() const
{
    return m_ui.asn1sccPathChooser->path();
}

void GeneralWidget::setAsn1SccPath(const QString &path)
{
    m_ui.asn1sccPathChooser->setPath(path);
}

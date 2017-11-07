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
#include "general.h"

#include <QWidget>

#include <asn1acnconstants.h>
#include <tr.h>

#include "generalwidget.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::OptionsPages;

General::General(Settings::GeneralPtr settings)
    : m_settings(settings),
      m_widget(nullptr)
{
    setId(Constants::GENERAL_SETTINGS_ID);
    setDisplayName(Tr::tr("General"));
    setCategory(Constants::SETTINGS_CATEGORY);
    setDisplayCategory(Tr::tr(Constants::SETTINGS_CATEGORY_DISPLAY));
    setCategoryIcon(Utils::Icon(Constants::OPTIONS_CATEGORY_ICON));
}

bool General::matches(const QString &searchKeyWord) const
{
    const QStringList keywords { "asn1scc", "asn1.exe", "asn1", "asn.1", "acn" };
    for (const auto& keyword : keywords)
        if (keyword.contains(searchKeyWord, Qt::CaseInsensitive))
            return true;
    return Core::IOptionsPage::matches(searchKeyWord);
}

QWidget* General::widget()
{
    if (!m_widget) {
        m_widget = new GeneralWidget;
        m_widget->setAsn1SccPath(m_settings->asn1sccPath());
    }
    return m_widget;
}

void General::apply()
{
    if (!m_widget)
        return;
    m_settings->setAsn1sccPath(m_widget->asn1sccPath());
    m_settings->changed();
    Settings::save(m_settings);
}

void General::finish()
{
    delete m_widget;
}

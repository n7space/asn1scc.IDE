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
#include "fuzzer.h"

#include <QWidget>
#include <QRegularExpression>

#include <tr.h>

#include "asn1acnconstants.h"
#include "fuzzerwidget.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::OptionsPages;

Fuzzer::Fuzzer(Settings::FuzzerPtr settings)
    : m_settings(settings)
    , m_widget(nullptr)
{
    setId(Constants::FUZZER_SETTINGS_ID);
    setDisplayName(Tr::tr("asn1scc.Fuzzer"));
    setCategory(Constants::SETTINGS_CATEGORY);
}

bool Fuzzer::matches(const QRegularExpression &regexp) const
{
    const QStringList keywords{"asn1", "asn.1", "acn", "test", "fuzzer", "asn1scc"};
    for (const auto &keyword : keywords)
        if (keyword.contains(regexp.pattern(), Qt::CaseInsensitive))
            return true;
    return Core::IOptionsPage::matches(regexp);
}

QWidget *Fuzzer::widget()
{
    if (!m_widget) {
        m_widget = new FuzzerWidget;
        m_widget->setPath(m_settings->path());
    }
    return m_widget;
}

void Fuzzer::apply()
{
    if (!m_widget)
        return;
    m_settings->setPath(m_widget->path());
    m_settings->changed();
    Settings::save(m_settings);
}

void Fuzzer::finish()
{
    delete m_widget;
}

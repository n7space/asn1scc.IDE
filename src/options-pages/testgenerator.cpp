/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
#include "testgenerator.h"

#include <QWidget>

#include <tr.h>

#include "asn1acnconstants.h"
#include "testgeneratorwidget.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::OptionsPages;

TestGenerator::TestGenerator(Settings::TestGeneratorPtr settings)
    : m_settings(settings)
    , m_widget(nullptr)
{
    setId(Constants::TESTGENERATOR_SETTINGS_ID);
    setDisplayName(Tr::tr("Test Generator"));
    setCategory(Constants::SETTINGS_CATEGORY);
    setDisplayCategory(Tr::tr(Constants::SETTINGS_CATEGORY_DISPLAY));
    setCategoryIcon(Utils::Icon(Constants::OPTIONS_CATEGORY_ICON));
}

bool TestGenerator::matches(const QString &searchKeyWord) const
{
    const QStringList keywords{"asn1", "asn.1", "acn", "test", "generator"};
    for (const auto &keyword : keywords)
        if (keyword.contains(searchKeyWord, Qt::CaseInsensitive))
            return true;
    return Core::IOptionsPage::matches(searchKeyWord);
}

QWidget *TestGenerator::widget()
{
    if (!m_widget) {
        m_widget = new TestGeneratorWidget;
        m_widget->setPath(m_settings->path());
    }
    return m_widget;
}

void TestGenerator::apply()
{
    if (!m_widget)
        return;
    m_settings->setPath(m_widget->path());
    m_settings->changed();
    Settings::save(m_settings);
}

void TestGenerator::finish()
{
    delete m_widget;
}

/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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
#include "service.h"

#include <QWidget>

#include "../asn1acnconstants.h"
#include "../tr.h"

#include "servicewidget.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::OptionsPages;

Service::Service(Settings::ServicePtr settings)
    : m_settings(settings)
{
    setId(Constants::SERVICE_SETTINGS_ID);
    setDisplayName(Tr::tr("Service"));
    setCategory(Constants::SETTINGS_CATEGORY);
    setDisplayCategory(Tr::tr(Constants::SETTINGS_CATEGORY_DISPLAY));
    setCategoryIcon(Utils::Icon(Constants::OPTIONS_CATEGORY_ICON));
}

bool Service::matches(const QString &searchKeyWord) const
{
    const QStringList keywords { "asn1scc", "daemon", "asn1.exe", "asn1", "asn.1", "acn" };
    for (const auto& keyword : keywords)
        if (keyword.contains(searchKeyWord, Qt::CaseInsensitive))
            return true;
    return Core::IOptionsPage::matches(searchKeyWord);
}

QWidget* Service::widget()
{
    if (!m_widget) {
        m_widget = new ServiceWidget;
        m_widget->setPath(m_settings->path);
        m_widget->setBaseUri(m_settings->baseUri);
        m_widget->setStayAlivePeriod(m_settings->stayAlivePeriod);
    }
    return m_widget;
}

void Service::apply()
{
    if (!m_widget)
        return;
    m_settings->path = m_widget->path();
    m_settings->baseUri = m_widget->baseUri();
    m_settings->stayAlivePeriod = m_widget->stayAlivePeriod();
    // TODO signal!
    Settings::save(m_settings);
}

void Service::finish()
{
    delete m_widget;
    m_widget = nullptr;
}

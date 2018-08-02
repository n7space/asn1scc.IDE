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

#include <QPointer>

#include <coreplugin/dialogs/ioptionspage.h>

#include <settings/testgenerator.h>

#include "ui_testgenerator.h"

namespace Asn1Acn {
namespace Internal {
namespace OptionsPages {

class TestGeneratorWidget;

class TestGenerator : public Core::IOptionsPage
{
public:
    TestGenerator(Settings::TestGeneratorPtr settings);

    bool matches(const QString &searchKeyWord) const override;
    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    Settings::TestGeneratorPtr m_settings;
    QPointer<TestGeneratorWidget> m_widget;
};

} // namespace OptionsPages
} // namespace Internal
} // namespace Asn1Acn

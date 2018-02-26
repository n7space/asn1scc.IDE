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
#pragma once

#include <projectexplorer/kitconfigwidget.h>
#include <utils/pathchooser.h>

namespace Asn1Acn {
namespace Internal {

class KitConfigWidget : public ProjectExplorer::KitConfigWidget
{
    Q_OBJECT
public:
    KitConfigWidget(ProjectExplorer::Kit *kit, const ProjectExplorer::KitInformation *ki);
    ~KitConfigWidget();

    QString displayName() const override;
    QString toolTip() const override;
    void makeReadOnly() override;
    void refresh() override;

    QWidget *mainWidget() const override;
    QWidget *buttonWidget() const override;

    void setPalette(const QPalette &p) override;

private:
    void pathWasChanged();

    Utils::PathChooser *m_chooser;
    bool m_ignoreChange = false;
};

} // namespace Internal
} // namespace Asn1Acn

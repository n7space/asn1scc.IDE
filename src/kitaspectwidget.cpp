/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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
#include "kitaspectwidget.h"

#include "kitaspect.h"

using namespace Asn1Acn::Internal;

KitAspectWidget::KitAspectWidget(ProjectExplorer::Kit *kit, const ProjectExplorer::KitAspect *ka)
    : ProjectExplorer::KitAspectWidget(kit, ka)
    , m_chooser(new Utils::PathChooser)
{
    m_chooser->setToolTip(tr("Path to ASN1SCC executable."));
    m_chooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_chooser->setHistoryCompleter(QLatin1String("Asn1Acn.Asn1Exe.History"));
    m_chooser->setFilePath(KitAspect::asn1Exe(kit));
    connect(m_chooser, &Utils::PathChooser::pathChanged, this, &KitAspectWidget::pathWasChanged);
}

KitAspectWidget::~KitAspectWidget()
{
    delete m_chooser;
}

void KitAspectWidget::pathWasChanged()
{
    m_ignoreChange = true;
    KitAspect::setAsn1Exe(m_kit, m_chooser->filePath());
    m_ignoreChange = false;
}

void KitAspectWidget::refresh()
{
    if (!m_ignoreChange)
        m_chooser->setFilePath(KitAspect::asn1Exe(m_kit));
}

void KitAspectWidget::makeReadOnly()
{
    m_chooser->setReadOnly(true);
}

QWidget *KitAspectWidget::mainWidget() const
{
    return m_chooser->lineEdit();
}

QWidget *KitAspectWidget::buttonWidget() const
{
    return m_chooser->buttonAtIndex(0);
}

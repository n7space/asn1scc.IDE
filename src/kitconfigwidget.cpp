/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
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
#include "kitconfigwidget.h"

#include "kitinformation.h"

using namespace Asn1Acn::Internal;

KitConfigWidget::KitConfigWidget(ProjectExplorer::Kit *kit,
                                 const ProjectExplorer::KitInformation *ki)
    : ProjectExplorer::KitConfigWidget(kit, ki)
{
    m_chooser = new Utils::PathChooser;
    m_chooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_chooser->setHistoryCompleter(QLatin1String("Asn1Acn.Asn1Exe.History"));
    m_chooser->setFileName(KitInformation::asn1Exe(kit));
    connect(m_chooser, &Utils::PathChooser::pathChanged, this, &KitConfigWidget::pathWasChanged);
}

KitConfigWidget::~KitConfigWidget()
{
    delete m_chooser;
}

void KitConfigWidget::pathWasChanged()
{
    m_ignoreChange = true;
    KitInformation::setAsn1Exe(m_kit, m_chooser->fileName());
    m_ignoreChange = false;
}

QString KitConfigWidget::displayName() const
{
    return tr("ASN1SCC:");
}

QString KitConfigWidget::toolTip() const
{
    return tr("Path to ASN1SCC executable.");
}

void KitConfigWidget::setPalette(const QPalette &p)
{
    ProjectExplorer::KitConfigWidget::setPalette(p);
    m_chooser->setOkColor(p.color(QPalette::Active, QPalette::Text));
}

void KitConfigWidget::refresh()
{
    if (!m_ignoreChange)
        m_chooser->setFileName(KitInformation::asn1Exe(m_kit));
}

void KitConfigWidget::makeReadOnly()
{
    m_chooser->setReadOnly(true);
}

QWidget *KitConfigWidget::mainWidget() const
{
    return m_chooser->lineEdit();
}

QWidget *KitConfigWidget::buttonWidget() const
{
    return m_chooser->buttonAtIndex(0);
}

/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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
#include "servicewidget.h"

#include <QRegularExpressionValidator>

using namespace Asn1Acn::Internal::OptionsPages;

ServiceWidget::ServiceWidget(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    m_ui.pathChooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
    const auto uriRegExp = QRegularExpression(
        "https?://([a-zA-Z0-9._\\-]+|[+])(:[0-9]+)?(/[a-zA-Z0-9._/\\-]+)?/");
    m_ui.uriEdit->setValidator(new QRegularExpressionValidator(uriRegExp, this));
    connect(m_ui.uriEdit, &QLineEdit::textChanged, [this](const QString &) {
        m_ui.uriEdit->setStyleSheet(
            m_ui.uriEdit->hasAcceptableInput() ? QLatin1String() : QLatin1String("color: red;"));
    });
}

QString ServiceWidget::path() const
{
    return m_ui.pathChooser->path();
}

void ServiceWidget::setPath(const QString &path)
{
    m_ui.pathChooser->setPath(path);
}

QString ServiceWidget::listeningUri() const
{
    const auto uri = m_ui.uriEdit->text();
    if (uri.isEmpty())
        return QString();
    if (uri.endsWith("/"))
        return uri;
    return uri + "/";
}

void ServiceWidget::setListeningUri(const QString &uri)
{
    m_ui.uriEdit->setText(uri);
}

int ServiceWidget::stayAlivePeriod() const
{
    if (m_ui.stayAliveCheckbox->isChecked())
        return -1;
    return m_ui.stayAliveSpinBox->value();
}

void ServiceWidget::setStayAlivePeriod(int i)
{
    if (i > 0) {
        m_ui.stayAliveCheckbox->setChecked(false);
        m_ui.stayAliveSpinBox->setValue(i);
    } else {
        m_ui.stayAliveCheckbox->setChecked(true);
        m_ui.stayAliveSpinBox->setValue(m_ui.stayAliveSpinBox->minimum());
    }
}

bool ServiceWidget::watchdogDisabled() const
{
    return m_ui.stayAliveCheckbox->isChecked();
}

void ServiceWidget::setWatchdogDisabled(bool checked)
{
    m_ui.stayAliveCheckbox->setChecked(checked);
}

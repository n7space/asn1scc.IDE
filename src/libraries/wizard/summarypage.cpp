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

#include "summarypage.h"

#include <QBoxLayout>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

SummaryPage::SummaryPage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_importer(importer)
{
    setTitle(QLatin1String("Summary"));

    m_directoryCaption = new QLabel(QLatin1String("Path: "), this);
    m_directory = new QLabel(this);

    m_filesListCaption = new QLabel(QLatin1String("Files to import: "), this);
    m_filesList = new QLabel(this);

    auto layout = new QBoxLayout(QBoxLayout::TopToBottom);

    layout->addWidget(m_directoryCaption);
    layout->addWidget(m_directory);

    layout->addWidget(m_filesListCaption);
    layout->addWidget(m_filesList);

    setLayout(layout);
}

void SummaryPage::initializePage()
{
    m_directory->setText(m_importer.path());

    QString filesText;
    for (auto it = m_importer.files().begin(); it != m_importer.files().end(); it++) {
        filesText += *it;
        if (it + 1 != m_importer.files().end())
            filesText += QLatin1String(",\n");
    }

    m_filesList->setText(filesText);
}

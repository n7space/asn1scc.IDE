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

#include "asnstructuresview.h"

#include <QVBoxLayout>

#include <coreplugin/idocument.h>
#include <coreplugin/find/itemviewfind.h>
#include <coreplugin/editormanager/editormanager.h>

#include "asn1acnconstants.h"
#include "asneditor.h"

using namespace Asn1Acn::Internal;

AsnStructuresTreeView::AsnStructuresTreeView(QWidget *parent) :
    Utils::NavigationTreeView(parent)
{
}

void AsnStructuresViewWidget::refreshModel()
{
    // TODO: this is stubbed implementation, to be replaced by real one
    Core::IDocument *currentDoc = Core::EditorManager::currentDocument();
    if (currentDoc == nullptr)
        return;

    QString fileName = currentDoc->filePath().toString();

    std::shared_ptr<AsnParsedDocument> docPtr(new AsnParsedDocument(fileName,
                                                                    -1,
                                                                    QStringList(fileName)));

    m_model->setDocument(docPtr);
}

void AsnStructuresViewWidget::onCurrentEditorChanged(Core::IEditor *editor)
{
    Q_UNUSED(editor);

    refreshModel();
}

AsnStructuresViewWidget::AsnStructuresViewWidget() :
    m_treeView(new AsnStructuresTreeView(this)),
    m_model(new AsnOverviewModel)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    refreshModel();
    m_treeView->setModel(m_model);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            this, &AsnStructuresViewWidget::onCurrentEditorChanged);
}

AsnStructuresViewFactory::AsnStructuresViewFactory()
{
    setDisplayName(tr("Structures View"));
    setPriority(500);
    setId(Constants::ASN_STRUCTURES_VIEW_ID);
}

Core::NavigationView AsnStructuresViewFactory::createWidget()
{
    return Core::NavigationView(new AsnStructuresViewWidget);
}

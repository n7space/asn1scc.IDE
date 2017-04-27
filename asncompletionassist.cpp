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
#include "asncompletionassist.h"

#include <coreplugin/id.h>

#include <texteditor/codeassist/assistproposalitem.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/genericproposalmodel.h>

#include "asn1acnconstants.h"

using namespace Asn1Acn::Internal;

AsnCompletionAssistProcessor::AsnCompletionAssistProcessor()
  : m_memberIcon(QLatin1String(":/codemodel/images/member.png")) // TODO in C++ mode somehow icons are colored
{
    // TODO snippets setSnippetGroup(Constants::ASN1_SNIPPETS_GROUP_ID);
}

/*
    : m_snippetCollector(QString(), QIcon(":/texteditor/images/snippet.png"))
    , m_variableIcon(QLatin1String(":/codemodel/images/keyword.png"))
 * */

TextEditor::IAssistProposal *AsnCompletionAssistProcessor::perform(const TextEditor::AssistInterface *interface)
{
    // TODO Keyword*Processor stores interface in ScopedPointer, why?

    if (interface->reason() == TextEditor::IdleEditor)
        return nullptr;

    // TODO ugly placeholder code for future refactor
    QList<TextEditor::AssistProposalItemInterface *> proposals;

    // TODO those should be somehow loaded from some global model
    auto proposalItem = new TextEditor::AssistProposalItem;
    proposalItem->setText("My-TestStructure");
    proposalItem->setDetail("detail"); // optional
    proposalItem->setIcon(m_memberIcon);
    proposalItem->setOrder(0); // ?
    proposals << proposalItem;
    proposalItem = new TextEditor::AssistProposalItem;
    proposalItem->setText("MyTestStructure");
    proposalItem->setIcon(m_memberIcon);
    proposals << proposalItem;
    proposalItem = new TextEditor::AssistProposalItem;
    proposalItem->setText(interface->fileName());
    proposalItem->setIcon(m_memberIcon);
    proposals << proposalItem;

    TextEditor::GenericProposalModel *model = new TextEditor::GenericProposalModel;
    model->loadContent(proposals);
    TextEditor::IAssistProposal *proposal = new TextEditor::GenericProposal(findStartOfName(interface), model);
    return proposal;
}

int AsnCompletionAssistProcessor::findStartOfName(const TextEditor::AssistInterface *interface) const
{
    int pos = interface->position();

    QChar chr;
    do {
        chr = interface->characterAt(--pos);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('-'));

    return ++pos;
}

bool AsnCompletionAssistProvider::supportsEditor(Core::Id editorId) const
{
    return editorId == Constants::ASNEDITOR_ID;
}

TextEditor::IAssistProcessor *AsnCompletionAssistProvider::createProcessor() const
{
    return new AsnCompletionAssistProcessor;
}

bool AsnCompletionAssistProvider::isContinuationChar(const QChar &c) const
{
    return c.isLetterOrNumber() || c == QLatin1Char('-');
}

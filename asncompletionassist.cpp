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

#include <memory>

#include <coreplugin/id.h>

#include <texteditor/codeassist/assistproposalitem.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/genericproposalmodel.h>

#include "asn1acnconstants.h"
#include "parseddatastorage.h"
#include "parseddocument.h"

#include "asnproposalsitemsprovider.h"

using namespace Asn1Acn::Internal;

static const int ASN_COMPLETER_ACTIVATION_LENGHT = 3;

AsnCompletionAssistProcessor::AsnCompletionAssistProcessor()
    : m_snippetCollector(QLatin1String(Constants::ASN1_SNIPPETS_GROUP_ID), QIcon(":/texteditor/images/snippet.png"))
{
}

TextEditor::IAssistProposal *AsnCompletionAssistProcessor::perform(const TextEditor::AssistInterface *interface)
{
    if (interface->reason() != TextEditor::ExplicitlyInvoked && !shouldAccept(interface))
        return nullptr;

    QList<TextEditor::AssistProposalItemInterface *> proposals;

    appendProposalsFromUserTypes(proposals, interface->fileName());
    appendProposalsFromSnippets(proposals);
    appendProposalsFromKeywords(proposals);

    TextEditor::IAssistProposal *proposal = new TextEditor::GenericProposal(findStartOfName(interface), proposals);

    delete interface;

    return proposal;
}

void
AsnCompletionAssistProcessor::appendProposalsFromUserTypes(QList<TextEditor::AssistProposalItemInterface *> &proposals, const QString &fileName) const
{
    ParsedDataStorage *storage = ParsedDataStorage::instance();

    std::shared_ptr<ParsedDocument> doc = storage->getFileForPath(fileName);
    if (doc == nullptr)
        return;

    proposals.append(doc->getProposalsProvider().takeProposals());
}

void AsnCompletionAssistProcessor::appendProposalsFromSnippets(QList<TextEditor::AssistProposalItemInterface *> &proposals) const
{
    proposals.append(m_snippetCollector.collect());
}

void AsnCompletionAssistProcessor::appendProposalsFromKeywords(QList<TextEditor::AssistProposalItemInterface *> &proposals) const
{
    // TODO: add ACN support
    AsnProposalBuiltinsProvider asnProvider;
    proposals.append(asnProvider.takeProposals());
}

bool AsnCompletionAssistProcessor::shouldAccept(const TextEditor::AssistInterface *interface)
{
    return (interface->position() - findStartOfName(interface) >= ASN_COMPLETER_ACTIVATION_LENGHT);
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

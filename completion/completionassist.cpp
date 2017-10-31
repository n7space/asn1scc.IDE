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
#include "completionassist.h"

#include <QIcon>

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>

#include <texteditor/codeassist/assistproposalitem.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/genericproposalmodel.h>

#include "data/file.h"

#include "asn1acnconstants.h"
#include "parseddatastorage.h"
#include "acneditor.h"
#include "asneditor.h"

#include "usertypesproposalsbuilder.h"

using namespace Asn1Acn::Internal::Completion;

static const int COMPLETER_ACTIVATION_LENGHT = 2;

CompletionAssistProcessor::CompletionAssistProcessor(const QString &snippetsGroup)
    : m_snippetCollector(snippetsGroup, QIcon(":/texteditor/images/snippet.png"))
{
}

TextEditor::IAssistProposal *CompletionAssistProcessor::perform(const TextEditor::AssistInterface *interface)
{
    if (interface->reason() != TextEditor::ExplicitlyInvoked && !shouldAccept(interface))
        return nullptr;

    Proposals proposals;

    appendProposalsFromUserTypes(proposals, interface->fileName());
    appendProposalsFromSnippets(proposals);
    appendProposalsFromKeywords(proposals);

    TextEditor::IAssistProposal *proposal = new TextEditor::GenericProposal(findStartOfName(interface), proposals);

    delete interface;

    return proposal;
}

void CompletionAssistProcessor::appendProposalsFromUserTypes(Proposals &proposals, const QString &fileName) const
{
    ParsedDataStorage *storage = ParsedDataStorage::instance();

    const auto file = storage->getAnyFileForPath(fileName);
    if (file == nullptr)
        return;

    Completion::UserTypesProposalsBuilder builder(file);
    proposals.append(builder.buildProposals());
}

void CompletionAssistProcessor::appendProposalsFromSnippets(Proposals &proposals) const
{
    proposals.append(m_snippetCollector.collect());
}

void CompletionAssistProcessor::appendProposalsFromKeywords(Proposals &proposals) const
{
    auto builder = createKeywordsProposalsBuilder();
    proposals.append(builder->buildProposals());
}

bool CompletionAssistProcessor::shouldAccept(const TextEditor::AssistInterface *interface) const
{
    return (interface->position() - findStartOfName(interface) >= COMPLETER_ACTIVATION_LENGHT);
}

int CompletionAssistProcessor::findStartOfName(const TextEditor::AssistInterface *interface) const
{
    int pos = interface->position();

    QChar chr;
    do {
        chr = interface->characterAt(--pos);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('-'));

    return ++pos;
}

bool CompletionAssistProvider::isContinuationChar(const QChar &c) const
{
    return c.isLetterOrNumber() || c == QLatin1Char('-');
}

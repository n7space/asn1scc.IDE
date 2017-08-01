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

#pragma once

#include <memory>

#include <texteditor/codeassist/assistinterface.h>
#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/iassistprocessor.h>
#include <texteditor/snippets/snippetassistcollector.h>

#include "proposalsbuiltinitemsprovider.h"

namespace Asn1Acn {
namespace Internal {

class CompletionAssistProcessor : public TextEditor::IAssistProcessor
{
public:
    CompletionAssistProcessor(const QString &snippetsGroup);

    TextEditor::IAssistProposal *perform(const TextEditor::AssistInterface *interface) override;

private:
    virtual std::unique_ptr<ProposalBuiltinsProvider> getBuiltinsProposalsProvider() const = 0;

    void appendProposalsFromUserTypes(QList<TextEditor::AssistProposalItemInterface *> &proposals, const QString &fileName) const;
    void appendProposalsFromSnippets(QList<TextEditor::AssistProposalItemInterface *> &proposals) const;
    void appendProposalsFromKeywords(QList<TextEditor::AssistProposalItemInterface *> &proposals) const;

    bool shouldAccept(const TextEditor::AssistInterface *interface) const;
    int findStartOfName(const TextEditor::AssistInterface *interface) const;

    TextEditor::SnippetAssistCollector m_snippetCollector;
};

class CompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT

public:
    bool isContinuationChar(const QChar &c) const override;
};

} // Internal
} // Asn1Acn

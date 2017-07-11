/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "documentsourceinfo.h"

using namespace Asn1Acn::Internal;

DocumentSourceInfo::DocumentSourceInfo(int revision, const QString &content, const QString &path, const QString &name) :
    m_revision(revision),
    m_rawContent(content),
    m_filePath(path),
    m_fileName(name)
{
}

int DocumentSourceInfo::getRevision() const
{
    return m_revision;
}

void DocumentSourceInfo::setRevision(int revision)
{
    m_revision = revision;
}

const QString &DocumentSourceInfo::getContent() const
{
    return m_rawContent;
}

void DocumentSourceInfo::setContent(QString content)
{
    m_rawContent = content;
}

const QString &DocumentSourceInfo::getName() const
{
    return m_fileName;
}

void DocumentSourceInfo::setName(QString name)
{
    m_fileName = name;
}

const QString &DocumentSourceInfo::getPath() const
{
    return m_filePath;
}

void DocumentSourceInfo::setPath(QString path)
{
    m_filePath = path;
}

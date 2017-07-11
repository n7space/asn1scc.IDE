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

#pragma once

#include <QString>
#include <QFileInfo>

namespace Asn1Acn {
namespace Internal {

class DocumentSourceInfo
{
public:
    DocumentSourceInfo() = default;
    DocumentSourceInfo(int revision, const QString &content, const QString &path, const QString &name);

    int getRevision() const;
    void setRevision(int revision);

    const QString &getContent() const;
    void setContent(QString content);

    const QString &getPath() const;
    void setPath(QString path);

    const QString &getName() const;
    void setName(QString name);

private:
    int m_revision;

    QString m_rawContent;
    QString m_filePath;
    QString m_fileName;
};

} // namespace Internal
} // namespace Asn1Acn

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

#pragma once

#include <QDialog>

#include <memory.h>

#include "ui_import_component.h"

#include <settings/libraries.h>

namespace Asn1Acn {
namespace Internal {
namespace ComponentImporter {

class ImportComponentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportComponentDialog(QWidget *parent = 0);

private slots:
    void accept() override;

    void refreshPaths();
    void builtInRadioToggled(bool checked);

private:
    void addFilesFromDirectory(const QString &directoryPath);
    QStringList pathsInDirectory(const QString &directoryPath);
    void addPathsToProject(const QStringList &paths);

    Ui::ImportComponentDialog m_ui;
    Settings::LibrariesConstPtr m_libraries;
};

} // namespace ComponentImporter
} // namespace Internal
} // namespace Asn1Acn

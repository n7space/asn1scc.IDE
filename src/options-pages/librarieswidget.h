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
#pragma once

#include <QStringList>
#include <QTreeWidgetItem>
#include <QWidget>

#include "ui_libraries.h"

namespace Asn1Acn {
namespace Internal {
namespace OptionsPages {

class LibrariesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LibrariesWidget(QWidget *parent = 0);
    ~LibrariesWidget();

    void setDetectedLibPaths(const QStringList &paths);
    void setManualLibPaths(const QStringList &paths);

    QStringList manualLibPaths() const;

private slots:
    void addExisitingLibraryDir();

private:
    class DetailsWidget;
    bool isManualItem(QTreeWidgetItem *item) const;
    QTreeWidgetItem *findManualItem(const QString &path) const;

    Ui::LibrariesOptionsPage m_ui;

    DetailsWidget *m_detailsWidget;

    QTreeWidgetItem *m_detectedRootItem;
    QTreeWidgetItem *m_manualRootItem;
};

} // namespace OptionsPages
} // namespace Internal
} // namespace Asn1Acn

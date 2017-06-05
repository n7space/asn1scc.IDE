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

#include <QWidget>

#include <coreplugin/inavigationwidgetfactory.h>
#include <coreplugin/editormanager/ieditor.h>

#include <utils/navigationtreeview.h>

#include "asnoverviewmodel.h"

namespace Asn1Acn {
namespace Internal {

class AsnStructuresTreeView : public Utils::NavigationTreeView
{
    Q_OBJECT
public:
    AsnStructuresTreeView(QWidget *parent);
};

// TODO: AsnStructuresViewWidget class should share base class with AsnOverview
class AsnStructuresViewWidget : public QWidget
{
    Q_OBJECT
public:
    AsnStructuresViewWidget();
    ~AsnStructuresViewWidget();

private:
    void onCurrentEditorChanged(Core::IEditor *editor);
    void refreshModel();
    void dataModelUpdated();

    Utils::NavigationTreeView *m_treeView;

    AsnOverviewModel *m_model;
    std::shared_ptr<AsnParsedObject> m_modelRoot;
};

class AsnStructuresViewFactory : public Core::INavigationWidgetFactory
{
    Q_OBJECT
public:
    AsnStructuresViewFactory();

    Core::NavigationView createWidget() override;
};

} // namespace Internal
} // namespace Asn1Acn

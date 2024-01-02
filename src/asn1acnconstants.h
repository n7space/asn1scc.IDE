/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <QtGlobal>

namespace Asn1Acn {
namespace Constants {

// Settings keys

const char SETTINGS_CATEGORY[] = "J.Asn1Acn";
const char SETTINGS_CATEGORY_DISPLAY[] = QT_TRANSLATE_NOOP("Asn1Acn", "ASN.1/ACN");
const char SERVICE_SETTINGS_ID[] = "C.Asn1Acn.Service";
const char LIBRARIES_SETTINGS_ID[] = "A.Asn1Acn.Libraries";
const char FUZZER_SETTINGS_ID[] = "B.Asn1Acn.Fuzzer";

const char SETTINGS_GROUP[] = "Asn1Acn";

// Shared constants

const char BASE_CONTEXT[] = "Asn1Acn.BaseContext";
const char CONTEXT_MENU[] = "Asn1Acn.ContextMenu";
const char WIZARD_CATEGORY[] = "O.Asn1Acn";
const char TYPESTREE_VIEW_ID[] = "Asn1Acn.TypesTreeView";

// ASN.1 constants

const char LANG_ASN1[] = "ASN.1";
const char ASNEDITOR_ID[] = "Asn1Acn.AsnEditor";
const char ASN1_MIMETYPE[] = "text/x-asn1";
const char ASN1_SNIPPETS_GROUP_ID[] = "ASN.1";

// ACN constants

const char LANG_ACN[] = "ACN";
const char ACNEDITOR_ID[] = "Asn1Acn.AcnEditor";
const char ACN_MIMETYPE[] = "text/x-acn";
const char ACN_SNIPPETS_GROUP_ID[] = "ACN";

// Menus

const char M_TOOLS_ASN[] = "Asn1Acn.Tools.Menu";
const char SWITCH_DATA_ENCODING[] = "Asn1Acn.SwitchDataEncoding";
const char OPEN_DATA_ENCODING_IN_NEXT_SPLIT[] = "Asn1Acn.SwitchDataEncodingInNextSplit";
const char IMPORT_FROM_COMPONENTS_LIBRARY_CONTEXT[] = "Asn1Acn.ImportFromComponentsLibraryContext";
const char IMPORT_FROM_COMPONENTS_LIBRARY_TOOLBAR[] = "Asn1Acn.ImportFromComponentsLibraryToolbar";
const char BUILD_ICD_CONTEXT[] = "Asn1Acn.BuildICDContext";
const char BUILD_ICD_TOOLBAR[] = "Asn1Acn.BuildICDToolbar";
const char FIND_USAGES[] = "Asn1Acn.FindUsages";
const char GENERATE_TESTS[] = "Asn1Acn.GenerateTests";

// Tasks

const char TASK_SEARCH[] = "Asn1Acn.Task.Search";

} // namespace Constants
} // namespace Asn1Acn

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

namespace Asn1Acn {
namespace Constants {

// Settings keys

const char SETTINGS_CATEGORY[]  = "J.Asn1Acn";
const char SETTINGS_CATEGORY_DISPLAY[] = QT_TRANSLATE_NOOP(Asn1Acn, "ASN.1/ACN");
const char GENERAL_SETTINGS_ID[] = "A.Asn1Acn.General";
const char SERVICE_SETTINGS_ID[] = "B.Asn1Acn.Service";

const char SETTINGS_GROUP[] = "Asn1Acn";

// Shared constants

const char CONTEXT_MENU[] = "Asn1Acn.ContextMenu";
const char WIZARD_CATEGORY[] = "O.Asn1Acn";
const char STRUCTURES_VIEW_ID[] = "Asn1Acn.StructuresView";

// ASN.1 constants

const char LANG_ASN1[] = "ASN.1";
const char ASNEDITOR_ID[] = "Asn1Acn.AsnEditor";
const char ASN1_MIMETYPE[] = "text/x-asn1";
const char ASN1_SNIPPETS_GROUP_ID[] = "ASN.1";

// ACN constants

const char LANG_ACN[] = "ACN";
const char ACNEDITOR_ID[] = "Asn1Acn.AcnEditor";
const char ACN_MIMETYPE[] = "text/x-acn";

// Icons

const char OPTIONS_CATEGORY_ICON[] = ":/asn1acn/images/options_category.png";

} // namespace Asn1Acn
} // namespace Constants

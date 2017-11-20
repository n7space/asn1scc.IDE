######################################################################
# Copyright (C) 2017 N7 Mobile sp. z o. o.
# Contact: http://n7mobile.pl/Space
#
# This file is part of ASN.1/ACN Plugin for QtCreator.
#
# Plugin was developed under a programme and funded by
# European Space Agency.
#
# This Plugin is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This Plugin is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#######################################################################

defineReplace(filterASN1ACNFiles) {
    allFiles = $${1}

    for(file, allFiles) {
        splitted = $$split(file, ".")
        extension = $$last(splitted)

        equals(extension, asn)|equals(extension, asn1)|equals(extension, acn) {
            fileNames += $${_PRO_FILE_PWD_}/$${file}
        }
    }

    return($$fileNames)
}

ASN1ACNFILES = $$filterASN1ACNFiles($${DISTFILES})

prepare.target += prepare
prepare.commands += $$sprintf($$QMAKE_MKDIR_CMD, $$ASN1SCC_PRODUCTS_DIR)
prepare.commands += $$sprintf($$QMAKE_MKDIR_CMD, $$ASN1SCC_ICD_DIR)

codeFromAsn1.target += codeFromAsn1
codeFromAsn1.commands += $$ASN1SCC $${ASN1SCC_GENERATING_OPTIONS} $$ASN1ACNFILES -o $$ASN1SCC_PRODUCTS_DIR
codeFromAsn1.depends += prepare

icdFromAsn1.target += icdFromAsn1
icdFromAsn1.commands += $$ASN1SCC $${ASN1SCC_ICD_OPTIONS} $${ASN1SCC_ICD_DIR}/$${ASN1SCC_ICD_FILE} $$ASN1ACNFILES
icdFromAsn1.depends += prepare

cleanGenerated.target += cleanGenerated
cleanGenerated.commands += $$QMAKE_DEL_TREE $$ASN1SCC_PRODUCTS_DIR
cleanGenerated.commands += $$QMAKE_DEL_TREE $$ASN1SCC_ICD_DIR
clean.depends += cleanGenerated

QMAKE_EXTRA_TARGETS += prepare codeFromAsn1 icdFromAsn1 cleanGenerated clean
PRE_TARGETDEPS += prepare codeFromAsn1

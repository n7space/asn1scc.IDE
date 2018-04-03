######################################################################
# Copyright (C) 2017 N7 Space sp. z o. o.
# Contact: http://n7space.com
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

defineReplace(createASN1SCCInput) {
    allFiles = $${1}

    for(file, allFiles) {
        fileNames += $$shell_quote($$file)
    }

    return($$fileNames)
}

ASN1ACNFILES = $$filterASN1ACNFiles($${DISTFILES})
ASN1SCCINPUT = $$createASN1SCCInput($${ASN1ACNFILES})

codeFromAsn1.target += $${ASN1_MAIN_GENERATED_HEADER}
codeFromAsn1.commands += $$sprintf($$QMAKE_MKDIR_CMD, $$shell_quote($$ASN1SCC_SRC_DIR))
codeFromAsn1.commands += $$escape_expand(\\n\\t)
codeFromAsn1.commands += $$ASN1SCC $${ASN1SCC_GENERATION_OPTIONS} -o $$shell_quote($$ASN1SCC_SRC_DIR) $$ASN1SCCINPUT
codeFromAsn1.depends += $$ASN1ACNFILES

icdFromAsn1.target += icdFromAsn1
icdFromAsn1.commands += $$sprintf($$QMAKE_MKDIR_CMD, $$shell_quote($$ASN1SCC_ICD_DIR))
icdFromAsn1.commands += $$escape_expand(\\n\\t)
icdFromAsn1.commands += $$ASN1SCC $${ASN1SCC_ICD_OPTIONS} $$shell_quote($${ASN1SCC_ICD_DIR}/$${ASN1SCC_ICD_FILE}) $$ASN1SCCINPUT
icdFromAsn1.depends += $$ASN1ACNFILES

cleanGenerated.target += cleanGenerated
cleanGenerated.commands += - $$QMAKE_DEL_TREE $$shell_quote($$ASN1SCC_PRODUCTS_DIR)
clean.depends += cleanGenerated

QMAKE_EXTRA_TARGETS += codeFromAsn1 icdFromAsn1 cleanGenerated clean
PRE_TARGETDEPS += $${ASN1_MAIN_GENERATED_HEADER}

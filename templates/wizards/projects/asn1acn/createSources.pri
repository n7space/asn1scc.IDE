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

runAsn1Scc.target += runAsn1Scc
runAsn1Scc.commands += $$ASN1SCC -c -$${ASN1SCC_ENCODING_OPTIONS} $$ASN1ACNFILES -o $$ASN1SCC_PRODUCTS_DIR
runAsn1Scc.depends += prepare

cleanSources.target += cleanSources
cleanSources.commands += $$QMAKE_DEL_TREE $$ASN1SCC_PRODUCTS_DIR
clean.depends += cleanSources

QMAKE_EXTRA_TARGETS += prepare runAsn1Scc cleanSources clean
PRE_TARGETDEPS += prepare runAsn1Scc

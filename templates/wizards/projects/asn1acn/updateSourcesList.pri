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

defineReplace(createFileNames) {
    allFiles = $${1}

    for(file, allFiles) {
        fileBaseName = $$basename(file)
        splitted = $$split(fileBaseName, ".")
        extension = $$last(splitted)

        equals(extension, asn)|equals(extension, asn1) {
            fileNames += $$first(splitted)
        }
    }

    return($$fileNames)
}

defineReplace(createHeadersList) {
    headersNames = $${1}

    for(name, headersNames) {
        header = $${ASN1SCC_PRODUCTS_DIR}/$${name}.h
        headers += $$header
    }

    return($$headers)
}

defineReplace(createSourcesList) {
    sourcesNames = $${1}

    for(name, sourcesNames) {
        source = $${ASN1SCC_PRODUCTS_DIR}/$${name}.c
        sources += $$source
    }

    return($$sources)
}

names = $$createFileNames($$DISTFILES)

!isEmpty(names) {
    PERSISTENT_HEADERS = $${ASN1SCC_PRODUCTS_DIR}/asn1crt.h
    PERSISTENT_SOURCES = $${ASN1SCC_PRODUCTS_DIR}/asn1crt.c $${ASN1SCC_PRODUCTS_DIR}/real.c

    contains(ASN1SCC_GENERATION_OPTIONS, -ACN) {
        PERSISTENT_SOURCES += $${ASN1SCC_PRODUCTS_DIR}/acn.c
    }
}

SOURCES += $$createSourcesList($$names) $$PERSISTENT_SOURCES
HEADERS += $$createHeadersList($$names) $$PERSISTENT_HEADERS

INCLUDEPATH += $$ASN1SCC_PRODUCTS_DIR

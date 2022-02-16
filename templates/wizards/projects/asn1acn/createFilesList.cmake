######################################################################
# Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

function(tryCreateFile fileName)
    if (NOT ${ARGC} EQUAL 1)
        return()
    endif()

    if (NOT EXISTS ${fileName})
        file(WRITE ${fileName})
    endif()
endfunction()

function(tryCreateFiles fileNames)
    foreach(fileName ${fileNames})
        tryCreateFile(${fileName})
    endforeach()
endfunction()

function(appendPersistentListC)
    set(persistentFiles ${ASN1SCC_C_DIR}/asn1crt.c)

    list(FIND ASN1SCC_C_OPTIONS --acn-enc ACN_ENCODING_IDX)
    if (NOT ${ACN_ENCODING_IDX} EQUAL -1)
        set(persistentFiles ${persistentFiles} ${ASN1SCC_C_DIR}/asn1crt_encoding.c
            ${ASN1SCC_C_DIR}/asn1crt_encoding_acn.c ${ASN1SCC_C_DIR}/asn1crt_encoding_uper.c)
    endif()

    tryCreateFiles("${persistentFiles}")
    set(ASN1SCC_PRODUCTS ${ASN1SCC_PRODUCTS} ${persistentFiles} PARENT_SCOPE)
endfunction()

function(appendPersistentListAda)
    set(persistentFiles ${ASN1SCC_ADA_DIR}/adaasn1rtl.adb)

    tryCreateFiles("${persistentFiles}")
    set(ASN1SCC_PRODUCTS ${ASN1SCC_PRODUCTS} ${persistentFiles} PARENT_SCOPE)
endfunction()

function(appendVolatileList outDir targetExtension result)
    foreach(fileName ${ASN1ACNSOURCES})
        get_filename_component(extension ${fileName} EXT)
        if (${extension} STREQUAL ".asn" OR ${extension} STREQUAL ".asn1")
            get_filename_component(fileName ${fileName} NAME_WE)
            tryCreateFile(${outDir}/${fileName}.${targetExtension})
            set(result ${result} "${outDir}/${fileName}.${targetExtension}")
        endif()
    endforeach()
    set(${output} ${result} PARENT_SCOPE)
endfunction()

function(appendVolatileListC)
    appendVolatileList(${ASN1SCC_C_DIR} "c" volatileFiles)
    set(ASN1SCC_PRODUCTS ${ASN1SCC_PRODUCTS} ${volatileFiles} PARENT_SCOPE)
endfunction()

function(appendVolatileListAda)
    appendVolatileList(${ASN1SCC_ADA_DIR} "adb" volatileFiles)
    set(ASN1SCC_PRODUCTS ${ASN1SCC_PRODUCTS} ${volatileFiles} PARENT_SCOPE)
endfunction()


if(ASN1ACNSOURCES)
    if(ASN1SCC_GENERATE_C)
        appendPersistentListC()
        appendVolatileListC()
    endif()
    if(ASN1SCC_GENERATE_ADA)
        appendPersistentListAda()
        appendVolatileListAda()
    endif()
endif()

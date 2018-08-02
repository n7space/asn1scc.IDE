######################################################################
# Copyright (C) 2017-2018 N7 Space sp. z o. o.
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

add_custom_target(
    codeFromAsn1
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ASN1SCC_PRODUCTS_DIR}
    COMMAND ${ASN1SCC} ${ASN1SCC_GENERATION_OPTIONS} -o ${ASN1SCC_PRODUCTS_DIR} ${ASN1ACNSOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating code using Asn1Scc"
    VERBATIM)

if (TARGET ${TARGET_NAME})
    add_dependencies(${TARGET_NAME} codeFromAsn1)
endif()

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${ASN1SCC_PRODUCTS_DIR})
include_directories(${ASN1SCC_PRODUCTS_DIR})

add_custom_target(
  icdFromAsn1
  COMMAND ${CMAKE_COMMAND} -E make_directory ${ASN1SCC_ICD_DIR}
  COMMAND ${ASN1SCC} ${ASN1SCC_ICD_OPTIONS} ${ASN1SCC_ICD_DIR}/${ASN1SCC_ICD_FILE} ${ASN1ACNSOURCES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating ICD using Asn1Scc"
  VERBATIM)

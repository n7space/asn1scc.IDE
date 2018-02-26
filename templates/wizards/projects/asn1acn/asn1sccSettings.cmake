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

if(NOT ASN1SCC)
  set(ASN1SCC $ENV{ASN1SCC})
  if(NOT ASN1SCC)
    set(ASN1SCC asn1.exe)
  endif()
endif()

if(NOT ASN1SCC_GENERATION_OPTIONS)
    set(ASN1SCC_GENERATION_OPTIONS -c -ACN)
endif()

if(NOT ASN1SCC_PRODUCTS_DIR)
    set(ASN1SCC_PRODUCTS_DIR ${PROJECT_BINARY_DIR}/asn1sccGenerated/src)
endif()

if(NOT ASN1SCC_ICD_DIR)
    set(ASN1SCC_ICD_DIR ${PROJECT_BINARY_DIR}/asn1sccGenerated/icd)
endif()

if(NOT ASN1SCC_ICD_FILE)
    set(ASN1SCC_ICD_FILE ICD.html)
endif()

if(NOT ASN1SCC_ICD_OPTIONS)
    set(ASN1SCC_ICD_OPTIONS -icdAcn)
endif()

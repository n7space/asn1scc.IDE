cmake_minimum_required(VERSION 2.8)

project(%{JS: '%{ProjectName}'})

set(ASN1ACNSOURCES %{JS: if ('%{AddAsnFile}' === 'true' && '%{AddAcnFile}' === 'true')
                             '%{AsnFullFileName} %{AcnFullFileName}'
                         else if ('%{AddAsnFile}' === 'true' )
                             '%{AsnFullFileName}'
                         else if ('%{AddAcnFile}' === 'true')
                             '%{AcnFullFileName}'
                         else
                             ' ' })

include(asn1sccSettings.cmake)
include(createFilesList.cmake)

set(SOURCES ${ASN1SCC_CPP_PRODUCTS})

if(SOURCES)
    set(TARGET_NAME ${PROJECT_NAME})
    add_executable(${TARGET_NAME} ${SOURCES})
endif()

include(configureAsn1SccTarget.cmake)

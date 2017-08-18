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

include(asn1sccSettings.txt)
include(createFilesList.txt)

set(SOURCES ${ASN1SCC_CPP_PRODUCTS})

if(SOURCES)
    set(EXECUTABLE_NAME ${PROJECT_NAME})
    add_executable(${EXECUTABLE_NAME} ${SOURCES})
endif()

include(configureAsn1SccTarget.txt)

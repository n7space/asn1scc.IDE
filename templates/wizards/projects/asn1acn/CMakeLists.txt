cmake_minimum_required(VERSION 3.21)

project(%{JS: '%{ProjectName}'})

set(
    ASN1ACNSOURCES
    %{JS: if (%{AddAsnFile} && %{AddAcnFile})
        '%{AsnRelativePath}/%{AsnFile}\\\n    %{AcnRelativePath}/%{AcnFile}'
    else if (%{AddAsnFile})
        '%{AsnRelativePath}/%{AsnFile}'
    else if (%{AddAcnFile})
        '%{AcnRelativePath}/%{AcnFile}'
        })

include(asn1sccSettings.cmake)
include(createFilesList.cmake)

set(SOURCES ${ASN1ACNSOURCES} ${ASN1SCC_PRODUCTS})

if(SOURCES)
    set(TARGET_NAME ${PROJECT_NAME})
    add_library(${TARGET_NAME} ${SOURCES})
endif()

include(configureAsn1SccTarget.cmake)

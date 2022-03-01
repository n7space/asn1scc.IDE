cmake_minimum_required(VERSION 2.8)

project(%{JS: '%{ProjectName}'})

set(ASN1ACNSOURCES DISTFILES += \\
                       @if %{AddAsnFile} && %{AddAcnFile}
                           %{AsnRelativePath}/%{AsnFile} \\
                           %{AcnRelativePath}/%{AcnFile}
                       @elsif %{AddAsnFile}
                           %{AsnRelativePath}/%{AsnFile}
                       @elsif %{AddAcnFile}
                           %{AcnRelativePath}/%{AcnFile}
                       @endif)

include(asn1sccSettings.cmake)
include(createFilesList.cmake)

set(SOURCES ${ASN1ACNSOURCES} ${ASN1SCC_PRODUCTS})

if(SOURCES)
    set(TARGET_NAME ${PROJECT_NAME})
    add_library(${TARGET_NAME} ${SOURCES})
endif()

include(configureAsn1SccTarget.cmake)

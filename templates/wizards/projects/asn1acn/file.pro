TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += \\
@if %{AddAsnFile} && %{AddAcnFile}
    %{AsnRelativePath}/%{AsnFile} \\
    %{AcnRelativePath}/%{AcnFile}
@elsif %{AddAsnFile}
    %{AsnRelativePath}/%{AsnFile}
@elsif %{AddAcnFile}
    %{AcnRelativePath}/%{AcnFile}
@endif

include(handleAsn1AcnBuild.pri)

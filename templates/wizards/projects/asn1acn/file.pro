TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

%{JS: if (%{AddAsnFile} && %{AddAcnFile})
        'DISTFILES += \\\\\n    %{AsnRelativePath}/%{AsnFile} \\\\\n    %{AcnRelativePath}/%{AcnFile}'
      else if (%{AddAsnFile})
        'DISTFILES += \\\\\n    %{AsnRelativePath}/%{AsnFile}'
      else if (%{AddAcnFile})
        'DISTFILES += \\\\\n    %{AcnRelativePath}/%{AcnFile}'
}

include(handleAsn1AcnBuild.pri)

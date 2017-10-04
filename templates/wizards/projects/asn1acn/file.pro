TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

%{JS: if (%{AddAsnFile} && %{AddAcnFile})
        'DISTFILES += \\\\ \n %{AsnRelativePath}/%{AsnFile} \\\\ \n %{AcnRelativePath}/%{AcnFile}'
      else if (%{AddAsnFile})
        'DISTFILES += \\\\ \n %{AsnRelativePath}/%{AsnFile}'
      else if (%{AddAcnFile})
        'DISTFILES += \\\\ \n %{AcnRelativePath}/%{AcnFile}'
      else
        'DISTFILES += ' }

include(handleAsn1AcnBuild.pri)
